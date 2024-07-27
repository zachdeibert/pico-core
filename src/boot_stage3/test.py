#!/usr/bin/env python3
from __future__ import annotations
import argparse
import struct
import typing

FLASH_APP = slice(0x10001000, 0x10100000)
FLASH_DATA = slice(0x10100000, 0x10200000)


class Uf2Block(typing.NamedTuple):
    magic_start_0: int
    magic_start_1: int
    flags: int
    target_addr: int
    payload_size: int
    block_no: int
    num_blocks: int
    file_size: int
    data: bytes
    magic_end: int

    @staticmethod
    def decode(block: bytes) -> Uf2Block:
        uf2 = Uf2Block(*struct.unpack("<IIIIIIII476sI", block))
        if (
            uf2.magic_start_0 != 0x0A324655
            or uf2.magic_start_1 != 0x9E5D5157
            or uf2.flags != 0x00002000
            or uf2.file_size != 0xE48BFF56
            or uf2.magic_end != 0x0AB16F30
        ):
            raise ValueError("Invalid UF2 block")
        return uf2

    def encode(self: Uf2Block) -> bytes:
        return struct.pack("<IIIIIIII476sI", *self)


class Uf2File:
    blocks: list[Uf2Block]

    def __init__(self: Uf2File) -> None:
        self.blocks = []

    def load(self: Uf2File, filename: str) -> None:
        self.blocks.clear()
        with open(filename, "rb") as f:
            while True:
                data = f.read(512)
                if len(data) == 0:
                    break
                self.blocks.append(Uf2Block.decode(data))

    def save(self: Uf2File, filename: str) -> None:
        with open(filename, "wb") as f:
            for block in self.blocks:
                f.write(block.encode())

    def stage_update(self: Uf2File) -> None:
        for i, block in enumerate(self.blocks):
            if (
                block.target_addr >= FLASH_APP.start
                and block.target_addr < FLASH_APP.stop
            ):
                self.blocks[i] = block._replace(
                    target_addr=block.target_addr - FLASH_APP.stop + FLASH_DATA.start
                )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="generate a UF2 image to test the boot3 update process"
    )
    parser.add_argument("input", help="the input UF2 file")
    parser.add_argument("output", help="the output UF2 file")
    parsed = parser.parse_args()
    file = Uf2File()
    file.load(parsed.input)
    file.stage_update()
    file.save(parsed.output)
