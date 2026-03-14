import gdb
import re

class SpanPrinter:
    def __init__(self, val):
        self.val = val

    def children(self):
        data = self.val['data']
        count = self.val['count']

        for i in range(int(count)):
            yield (f"[{i}]", (data + i).dereference())

    def display_hint(self):
        return "array"

    def to_string(self):
        data = self.val['data']
        count = self.val['count']

        if re.match(r"^tl::Span<char8_t, .*>$", str(self.val.type)):
            return '"' + str(gdb.selected_inferior().read_memory(data, count).tobytes().decode('utf-8', 'ignore')) + '"'
        return f"Span<{str(data.type.target().strip_typedefs())}>"

class ListPrinter:
    def __init__(self, val):
        self.val = val

    def children(self):
        data = self.val['data']
        count = self.val['count']
        
        for i in range(int(count)):
            yield (f"[{i}]", (data + i).dereference())

    def display_hint(self):
        return "map"

    def to_string(self):
        data = self.val['data']
        count = self.val['count']

        if re.match(r"^tl::List<char8_t, .*>$", str(self.val.type)):
            return '"' + str(gdb.selected_inferior().read_memory(data, count).tobytes().decode('utf-8', 'ignore')) + '"'
        return f"List<{str(data.type.target().strip_typedefs())}>"


def my_lookup_function(val):
    if re.match(r"^tl::Span<.*>$", str(val.type)):
        return SpanPrinter(val)
    if re.match(r"^tl::List<.*>$", str(val.type)):
        return ListPrinter(val)
    return None

gdb.pretty_printers.append(my_lookup_function)