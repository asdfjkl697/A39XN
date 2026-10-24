#include "S0553Key.h"
#include "config.h"

#ifdef USE_S0553

#include "gd32f30x.h"
#include "Display.h"
#include "KeyFunc.h"
#include "Leg.h"
#include "MassageInit.h"
#include "Motor.h"
#include "S0553_HAL.h"
#include "S0553_Host.h"
#include "S0553_main.h"
#include "WorkState.h"
#include "MoveLogic.h"

#define S0553_PUSH_INTERVAL_TICKS 50u
#define S0553_PROGRAM_MAX 20u
#define S0553_LOWER_WISE_COUNT 7u
#define S0553_UART_BUFFER_SIZE 512u
#define S0553_UART_BUFFER_MASK (S0553_UART_BUFFER_SIZE - 1u)
#define S0553_UART_BIT_LENGTH 9u

/* Select 9600u or 115200u; timer runs at 120 MHz with 3x UART sampling. */
#ifndef S0553_UART_BAUD
#define S0553_UART_BAUD 9600u
#endif

#if S0553_UART_BAUD == 9600u
#define S0553_UART_TIMER_PERIOD 4166u
#elif S0553_UART_BAUD == 115200u
#define S0553_UART_TIMER_PERIOD 346u
#else
#error S0553_UART_BAUD must be 9600 or 115200
#endif

static volatile uint8_t s_UartRxBuffer[S0553_UART_BUFFER_SIZE];
static volatile uint8_t s_UartTxBuffer[S0553_UART_BUFFER_SIZE];
static volatile uint16_t s_UartRxWrite;
static volatile uint16_t s_UartRxRead;
static volatile uint16_t s_UartTxWrite;
static volatile uint16_t s_UartTxRead;
static volatile uint8_t s_UartTxBusy;
static volatile uint8_t s_UartRxActive;
static volatile uint8_t s_UartTxSample;
static volatile uint8_t s_UartRxSample;
static volatile uint8_t s_UartTxBits;
static volatile uint8_t s_UartRxBits;
static volatile uint8_t s_UartRxTimeout;
static volatile uint8_t s_UartTxShift;
static volatile uint8_t s_UartRxShift;
volatile uint32_t S0553_TxFrameDropCount;

#define S0553_UART_RX_PIN gpio_input_bit_get(GPIOE, GPIO_PIN_13)
#define S0553_UART_TX_SET(value) gpio_bit_write(GPIOE, GPIO_PIN_14, (bit_status)(value))

static void S0553Key_UartSetOutput(void)
{
}

static void S0553Key_UartSetInput(void)
{
}

static void S0553Key_UartInit(void)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_TIMER4);

    gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_bit_set(GPIOE, GPIO_PIN_14);
    gpio_init(GPIOE, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_13);

    timer_deinit(TIMER4);
    timer_initpara.prescaler = 0;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = S0553_UART_TIMER_PERIOD;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER4, &timer_initpara);

    timer_flag_clear(TIMER4, TIMER_FLAG_UP);
    timer_interrupt_enable(TIMER4, TIMER_INT_UP);
    nvic_irq_enable(TIMER4_IRQn, 1, 0);

    s_UartRxWrite = 0u;
    s_UartRxRead = 0u;
    s_UartTxWrite = 0u;
    s_UartTxRead = 0u;
    s_UartTxBusy = 0u;
    s_UartRxActive = 0u;
    s_UartTxSample = 3u;
    s_UartRxSample = 4u;
    s_UartTxBits = 0u;
    s_UartRxBits = 0u;
    s_UartRxTimeout = 0u;
    s_UartTxShift = 0u;
    s_UartRxShift = 0u;
    S0553_TxFrameDropCount = 0u;

    timer_enable(TIMER4);
}

void TIMER4_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER4, TIMER_INT_FLAG_UP) != SET)
    {
        return;
    }

    timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_UP);
    if (s_UartTxBusy != 0u)
    {
        if (--s_UartTxSample == 0u)
        {
            s_UartTxSample = 3u;
            if (s_UartTxBits == 0u)
            {
                S0553_UART_TX_SET(0);
                s_UartTxShift = s_UartTxBuffer[s_UartTxRead];
                s_UartTxBits = S0553_UART_BIT_LENGTH;
            }
            else if (--s_UartTxBits == 0u)
            {
                S0553_UART_TX_SET(1);
                s_UartTxRead = (uint16_t)((s_UartTxRead + 1u) & S0553_UART_BUFFER_MASK);
                if (s_UartTxRead == s_UartTxWrite)
                {
                    s_UartTxBusy = 0u;
                    S0553Key_UartSetInput();
                }
            }
            else
            {
                S0553_UART_TX_SET(s_UartTxShift & 0x01u);
                s_UartTxShift >>= 1;
            }
        }
    }

    if (s_UartRxActive != 0u)
    {
        s_UartRxTimeout = 10u;
        if (--s_UartRxSample == 0u)
        {
            s_UartRxSample = 3u;
            if (--s_UartRxBits == 0u)
            {
                const uint16_t next = (uint16_t)((s_UartRxWrite + 1u) & S0553_UART_BUFFER_MASK);
                s_UartRxActive = 0u;
                if (next != s_UartRxRead)
                {
                    s_UartRxBuffer[s_UartRxWrite] = s_UartRxShift;
                    s_UartRxWrite = next;
                }
            }
            else
            {
                s_UartRxShift >>= 1;
                if (S0553_UART_RX_PIN)
                {
                    s_UartRxShift |= 0x80u;
                }
            }
        }
    }
    else if (!S0553_UART_RX_PIN)
    {
        s_UartRxActive = 1u;
        s_UartRxSample = 4u;
        s_UartRxBits = S0553_UART_BIT_LENGTH;
    }

    if (s_UartRxTimeout > 0u)
    {
        --s_UartRxTimeout;
    }
    else if (s_UartTxBusy == 0u &&
             s_UartRxActive == 0u &&
             s_UartTxRead != s_UartTxWrite)
    {
        S0553Key_UartSetOutput();
        s_UartTxBusy = 1u;
        s_UartTxBits = 0u;
    }
}

static uint8_t s_Connected;
static uint8_t s_PushTicks;
static uint8_t s_HadConnection;
static KeyFunc_E s_PostureKey;
static uint8_t s_PostureParam;
static uint8_t s_PostureActive;
volatile uint8_t S0553_ProbeFault;

static uint8_t S0553Key_WorkIsActive(void)
{
    const uint8_t state = WorkStateGet();
    return (state == WorkState_Work || state == WorkState_WaitFunc) ? 1u : 0u;
}

static uint8_t S0553Key_GetPowerValue(void)
{
    const uint8_t state = WorkStateGet();
    if (state == WorkState_Work)
    {
        return 5u;
    }
    if (state == WorkState_Wait || state == WorkState_Reset)
    {
        return 0u;
    }
    return 1u;
}

static uint8_t S0553Key_GetLanguage(void)
{
    switch (Dispaly_Language)
    {
    case Chinese:
        return 0u;
    case English:
        return 1u;
    case German:
        return 2u;
    default:
        return 1u;
    }
}

static uint8_t S0553Key_GetWiseId(uint8_t which)
{
    switch ((ManualWise_E)MassWiseGet(which))
    {
    case RNZ:
        return 1u;
    case PD:
        return 2u;
    case PDB:
        return 3u;
    case RPZ:
        return 4u;
    case ZYS:
        return 5u;
    case ZYD:
        return 6u;
    case JRZ:
        if (which == MoveDw)
            return 0u;
        return 7u;
    case D3RNZ:
        if (which == MoveDw)
            return 0u;
        return 8u;
    case D3PD:
        if (which == MoveDw)
            return 0u;
        return 9u;
    case D3RPZ:
        if (which == MoveDw)
            return 0u;
        return 10u;
    case D3RPF:
        if (which == MoveDw)
            return 0u;
        return 11u;
    case TZ:
    default:
        return 0u;
    }
}

static void S0553Key_PushAllDataPoints(void)
{
    uint8_t zeroGravity = (nCurZeroState <= 3u) ? nCurZeroState : 0u;
    uint8_t program = AutoModeGet();

    if (program > 25u)
    {
        program = 0u;
    }

    S0553_Host_NotifyValue(S0553_DP_TIME_MIN, WorkMinGet());
    S0553_Host_NotifyValue(S0553_DP_TIME_SEC, WorkSecGet());
    S0553_Host_NotifyValue(S0553_DP_PROGRAM, program);
    S0553_Host_NotifyValue(S0553_DP_WISE, S0553Key_GetWiseId(MoveUp));
#ifdef USE_M2
    S0553_Host_NotifyValue(S0553_DP_WISE_LOWER, S0553Key_GetWiseId(MoveDw));
#else
    S0553_Host_NotifyValue(S0553_DP_WISE_LOWER, 0u);
#endif
    S0553_Host_NotifyValue(S0553_DP_LANGUAGE, S0553Key_GetLanguage());
    S0553_Host_NotifyValue(S0553_DP_ZERO_GRAVITY, zeroGravity);
    S0553_Host_NotifyValue(S0553_DP_POWER, S0553Key_GetPowerValue());
}

static void S0553Key_DispatchState(KeyFunc_E key, uint8_t param, uint8_t state)
{
    const KeyFunc_S previous = Key;

    Key.Value = (uint8_t)key;
    Key.Param = param;
    Key.State = state;
    KeyFunc();
    Key = previous;
}

static void S0553Key_Dispatch(KeyFunc_E key, uint8_t param)
{
    S0553Key_DispatchState(key, param, 2u);
}

static uint8_t S0553Key_MapLanguage(uint8_t language, uint8_t *displayLanguage)
{
    switch (language)
    {
    case 0u:
        *displayLanguage = Chinese;
        return 1u;
    case 1u:
        *displayLanguage = English;
        return 1u;
    case 2u:
        *displayLanguage = German;
        return 1u;
    default:
        return 0u;
    }
}

static uint8_t S0553Key_MapWiseCommand(int16_t wiseId, uint8_t *keyParam)
{
    switch (wiseId)
    {
    case 0:
        *keyParam = 1u; /* TZ */
        return 1u;
    case 1:
        *keyParam = 2u; /* RNZ */
        return 1u;
    case 2:
        *keyParam = 3u; /* PD */
        return 1u;
    case 3:
        *keyParam = 5u; /* RPZ */
        return 1u;
    case 4:
        *keyParam = 4u; /* PDB */
        return 1u;
    case 5:
        *keyParam = 6u; /* ZYS */
        return 1u;
    case 8:
        *keyParam = 7u; /* ZYD */
        return 1u;
    case 9:
        *keyParam = 8u; /* D3RNZ */
        return 1u;
    case 10:
        *keyParam = 9u; /* D3PD */
        return 1u;
    case 11:
        *keyParam = 10u; /* D3RPZ */
        return 1u;
    default:
        return 0u;
    }
}

static void S0553Key_SetWise(uint8_t which, int16_t arg)
{
    const uint8_t previous = MoveWhichGet();
    uint8_t keyParam;

    if (!S0553Key_MapWiseCommand(arg, &keyParam) || !S0553Key_WorkIsActive())
    {
        return;
    }

    MoveWhichSet(which);
    S0553Key_Dispatch(Key_Wise, keyParam);
    MoveWhichSet(previous);
}

static void S0553Key_StopPosture(void)
{
    if (s_PostureActive == 0u)
    {
        return;
    }

    /* KeyFunc's release path stops the active posture actuator. */
    S0553Key_DispatchState(s_PostureKey, s_PostureParam, 2u);
    s_PostureActive = 0u;
}

static uint8_t S0553Key_MapPosture(int16_t posture, KeyFunc_E *key, uint8_t *param)
{
    switch (posture)
    {
    case S0553_POSTURE_LINKAGE_UP:
        *key = Key_Lying;
        *param = 1u;
        return 1u;
    case S0553_POSTURE_LINKAGE_DOWN:
        *key = Key_Lying;
        *param = 2u;
        return 1u;
    case S0553_POSTURE_BACKREST_UP:
        *key = Key_BackUpDown;
        *param = 1u;
        return 1u;
    case S0553_POSTURE_BACKREST_DOWN:
        *key = Key_BackUpDown;
        *param = 2u;
        return 1u;
    case S0553_POSTURE_CALF_UP:
        *key = Key_LegsUpDown;
        *param = 2u;
        return 1u;
    case S0553_POSTURE_CALF_DOWN:
        *key = Key_LegsUpDown;
        *param = 1u;
        return 1u;
    case S0553_POSTURE_FOOT_UP:
        *key = Key_LegsFlex;
        *param = 2u;
        return 1u;
    case S0553_POSTURE_FOOT_DOWN:
        *key = Key_LegsFlex;
        *param = 1u;
        return 1u;
    default:
        return 0u;
    }
}

void S0553Host_OnCommand(S0553HostCommand cmd, S0553DataPoint dp, int16_t arg)
{
    uint8_t value;

    switch (dp)
    {
    /* ---------- 非数据点命令：坐姿动作（联动/靠背/小腿/足部 × 升/降 = 8 种，持续运行型） ---------- */
    case S0553_DP_NONE:
    {
        /* Posture commands are press/release pairs routed through the existing key logic. */
        if (cmd == S0553_HCMD_POSTURE_STOP)
        {
            if (arg == 0)
            {
                S0553Key_StopPosture();
            }
        }
        else if (cmd == S0553_HCMD_POSTURE)
        {
            KeyFunc_E key;
            uint8_t param;

            if (!S0553Key_WorkIsActive() || !S0553Key_MapPosture(arg, &key, &param))
                break;

            if (s_PostureActive != 0u && s_PostureKey == key && s_PostureParam == param)
            {
                break;
            }

            /* Release the previous direction before starting the newly requested one. */
            S0553Key_StopPosture();
            S0553Key_DispatchState(key, param, 1u);
            s_PostureKey = key;
            s_PostureParam = param;
            s_PostureActive = 1u;
        }
        break;
    }

    case S0553_DP_PROGRAM:
        if (arg < 0 || arg > S0553_PROGRAM_MAX)
        {
            break;
        }
        if (arg == 0)
        {
            S0553Key_Dispatch(Key_Wise, 0u);
        }
        else
        {
            S0553Key_Dispatch(Key_AutoMode, (uint8_t)arg);
        }
        break;

    case S0553_DP_WISE:
        S0553Key_SetWise(MoveUp, arg);
        break;

    case S0553_DP_WISE_LOWER:
#ifdef USE_M2
        if (arg >= 0 && arg < S0553_LOWER_WISE_COUNT)
        {
            S0553Key_SetWise(MoveDw, arg);
        }
#endif
        break;

    case S0553_DP_LANGUAGE:
        if (S0553Key_MapLanguage((uint8_t)arg, &value))
        {
            S0553Key_Dispatch(Key_Language, value);
        }
        break;

    case S0553_DP_ZERO_GRAVITY:
        if (arg >= 0 && arg <= 3 && S0553Key_WorkIsActive())
        {
            S0553Key_Dispatch(Key_Zero, (uint8_t)arg);
        }
        break;

    default:
        break;
    }
}

void S0553Key_Init(void)
{
    s_Connected = 0u;
    s_PushTicks = 0u;
    s_HadConnection = 0u;
    s_PostureActive = 0u;
    S0553_ProbeFault = 0u;
    S0553_Init();
}

void S0553Key_Task10ms(void)
{
    S0553_Task();

    if (!S0553_IsConnected())
    {
        s_Connected = 0u;
        if (s_HadConnection != 0u)
        {
            S0553_ProbeFault = 1u;
        }
        return;
    }

    s_HadConnection = 1u;
    S0553_ProbeFault = 0u;

    if (s_Connected == 0u)
    {
        s_Connected = 1u;
        s_PushTicks = 0u;
        S0553Key_PushAllDataPoints();
        return;
    }

    if (++s_PushTicks >= S0553_PUSH_INTERVAL_TICKS)
    {
        s_PushTicks = 0u;
        S0553Key_PushAllDataPoints();
    }
}

uint8_t S0553_HAL_UartInit(void)
{
    S0553Key_UartInit();
    return S0553_HAL_OK;
}

uint16_t S0553_HAL_UartRxAvailable(void)
{
    const uint16_t read = s_UartRxRead;
    const uint16_t write = s_UartRxWrite;
    return (write >= read) ? (uint16_t)(write - read)
                           : (uint16_t)(write + S0553_UART_BUFFER_SIZE - read);
}

uint8_t S0553_HAL_UartReadByte(void)
{
    const uint8_t byte = s_UartRxBuffer[s_UartRxRead];
    s_UartRxRead = (uint16_t)((s_UartRxRead + 1u) & S0553_UART_BUFFER_MASK);
    return byte;
}

uint8_t S0553_HAL_UartWriteFrame(const uint8_t *data, uint16_t len)
{
    const uint16_t read = s_UartTxRead;
    uint16_t write = s_UartTxWrite;
    const uint16_t used = (write >= read)
                              ? (uint16_t)(write - read)
                              : (uint16_t)(write + S0553_UART_BUFFER_SIZE - read);
    const uint16_t available = (uint16_t)(S0553_UART_BUFFER_SIZE - 1u - used);

    if (data == NULL || len == 0u || len > available)
    {
        S0553_TxFrameDropCount++;
        return S0553_HAL_ERROR;
    }

    /* @ AI md for reboot 261024: publish only complete frames and never wait for TX ISR. */
    for (uint16_t i = 0u; i < len; i++)
    {
        s_UartTxBuffer[write] = data[i];
        write = (uint16_t)((write + 1u) & S0553_UART_BUFFER_MASK);
    }
    s_UartTxWrite = write;
    return S0553_HAL_OK;
}

void S0553_HAL_UartWriteByte(uint8_t byte)
{
    (void)S0553_HAL_UartWriteFrame(&byte, 1u);
}

void S0553_HAL_EnterCritical(void)
{
}

void S0553_HAL_ExitCritical(void)
{
}

#else

void S0553Key_Init(void)
{
}

void S0553Key_Task10ms(void)
{
}

#endif
