#!/usr/bin/env python3
"""Merge a bootloader Intel HEX image into an application HEX image."""

import os
import sys
import tempfile
from pathlib import Path


def read_hex(path):
    memory = {}
    start_records = []
    base = 0
    eof_seen = False

    for line_number, line in enumerate(path.read_text(encoding="ascii").splitlines(), 1):
        if eof_seen:
            raise ValueError(f"{path}:{line_number}: data found after EOF record")
        if not line.startswith(":"):
            raise ValueError(f"{path}:{line_number}: missing ':'")

        try:
            record = bytes.fromhex(line[1:])
        except ValueError as error:
            raise ValueError(f"{path}:{line_number}: invalid hex record") from error
        if len(record) < 5 or len(record) != record[0] + 5 or sum(record) & 0xFF:
            raise ValueError(f"{path}:{line_number}: invalid record length or checksum")

        length = record[0]
        address = int.from_bytes(record[1:3], "big")
        record_type = record[3]
        data = record[4:-1]

        if record_type == 0:
            absolute_address = base + address
            for offset, value in enumerate(data):
                location = absolute_address + offset
                if location in memory:
                    raise ValueError(f"{path}:{line_number}: duplicate address 0x{location:X}")
                memory[location] = value
        elif record_type == 1:
            if length or address:
                raise ValueError(f"{path}:{line_number}: malformed EOF record")
            eof_seen = True
        elif record_type == 2:
            if length != 2 or address:
                raise ValueError(f"{path}:{line_number}: malformed segment address record")
            base = int.from_bytes(data, "big") << 4
        elif record_type == 3:
            if length != 4 or address:
                raise ValueError(f"{path}:{line_number}: malformed start segment record")
            start_records.append((record_type, data))
        elif record_type == 4:
            if length != 2 or address:
                raise ValueError(f"{path}:{line_number}: malformed linear address record")
            base = int.from_bytes(data, "big") << 16
        elif record_type == 5:
            if length != 4 or address:
                raise ValueError(f"{path}:{line_number}: malformed start linear record")
            start_records.append((record_type, data))
        else:
            raise ValueError(f"{path}:{line_number}: unsupported record type {record_type}")

    if not eof_seen:
        raise ValueError(f"{path}: missing EOF record")
    return memory, start_records


def make_record(address, record_type, data):
    contents = bytes((len(data),)) + address.to_bytes(2, "big") + bytes((record_type,)) + data
    checksum = (-sum(contents)) & 0xFF
    return ":" + (contents + bytes((checksum,))).hex().upper()


def merge(boot_path, app_path):
    boot_memory, boot_start = read_hex(boot_path)
    app_memory, app_start = read_hex(app_path)
    collisions = boot_memory.keys() & app_memory.keys()
    if collisions:
        address = min(collisions)
        raise ValueError(f"boot and application overlap at address 0x{address:X}")

    memory = {**boot_memory, **app_memory}
    output_records = []
    current_upper = None
    addresses = sorted(memory)
    index = 0

    while index < len(addresses):
        address = addresses[index]
        upper = address >> 16
        if upper != current_upper:
            output_records.append(make_record(0, 4, upper.to_bytes(2, "big")))
            current_upper = upper

        chunk = bytearray((memory[address],))
        index += 1
        while (
            index < len(addresses)
            and addresses[index] == address + len(chunk)
            and addresses[index] >> 16 == upper
            and len(chunk) < 16
        ):
            chunk.append(memory[addresses[index]])
            index += 1
        output_records.append(make_record(address & 0xFFFF, 0, bytes(chunk)))

    start_record = boot_start or app_start
    if start_record:
        record_type, data = start_record[0]
        output_records.append(make_record(0, record_type, data))
    output_records.append(make_record(0, 1, b""))

    app_path.parent.mkdir(parents=True, exist_ok=True)
    temporary_path = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w", encoding="ascii", newline="\n",
            dir=app_path.parent, delete=False
        ) as temporary:
            temporary_path = Path(temporary.name)
            temporary.write("\n".join(output_records) + "\n")
        os.replace(temporary_path, app_path)
    finally:
        if temporary_path is not None and temporary_path.exists():
            temporary_path.unlink()

    print(f"Merged {len(boot_memory)} boot bytes into {app_path} ({len(memory)} total bytes)")


def main():
    if len(sys.argv) != 3:
        print("Usage: merge_hex.py <boot.hex> <application.hex>", file=sys.stderr)
        return 2
    try:
        merge(Path(sys.argv[1]), Path(sys.argv[2]))
    except (OSError, ValueError) as error:
        print(f"HEX merge failed: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
