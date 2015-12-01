#!/usr/bin/python3
from random import randint


pat_fields = [
    ("table_id",                    8),
    ("section_syntax_indicator",    1),
    ("private_bit",                 1),
    ("reserved_bits",               2),
    ("section_length",              12),
    ("version",                     5),
    ("current_next_indicator",      1),
    ("section_number",              8),
    ("reserved_1",                  3),
    ("PCR_PID",                     13),
    ("reserved_2",                  4),
    ("program_info_len",            12),
]

section_fields = [
    ("stream_type",             8),
    ("reserved_1",              3),
    ("elementary_PID",          13),
    ("reserved_2",              4),
    ("es_info_len",             12)
]


class Field:
    def __init__(self, name, b_len, value):
        self.name = name
        self.b_len = b_len
        self.value = value

    def __str__(self):
        return "{} = {}".format(self.name, self.value)

    def get_raw(self):
        return bin(self.value)[2:]


class ConfigHeader:
    fields = []

    def __init__(self):
        for f in pat_fields:
            self.fields.append(Field(f[0], f[1], randint(0, 2**f[1])))

    def get_raw(self):
        data = ""

        for f in self.fields:
            data += f.get_raw()

        return data


class SectionEntry:
    fields = []

    def __init__(self):
        for f in section_fields:
            self.fields.append(Field(f[0], f[1], randint(0, 2**f[1])))

    def get_raw(self):
        data = ""

        for f in self.fields:
            data += f.get_raw()

        return data


class ConfigGen:
    header_fmt = "[pat {}]"

    header = None
    sections = []

    def generate(self):
        self.programs_cnt = randint(0, 32)

        self.header = ConfigHeader()
        self.sections = []

        for i in range(self.programs_cnt):
            self.sections.append(SectionEntry())

    def get_raw(self):
        data = ""
        data += self.header.get_raw()

        for s in self.sections:
            data += s.get_raw()

        return data

if __name__ == "__main__":
    c = ConfigGen()
    c.generate()
    print(c.get_raw())
