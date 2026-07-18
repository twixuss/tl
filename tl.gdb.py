import gdb
import re

class ArrayPrinter:
    def __init__(self, val):
        self.data = val['data']
        
        t = val.type.strip_typedefs()

        self.count = int(t.template_argument(1))
        
        Element = t.template_argument(0)
        if str(Element).startswith("tl::Array<"):
            self.to_string = self.to_string_outer
            self.inner_count = int(Element.strip_typedefs().template_argument(1))
        else:
            self.to_string = self.to_string_inner

    def to_string_inner(self):
        data = self.data
        count = self.count
        return "{" + ", ".join([str(data[i]) for i in range(count)]) + "}"

    def to_string_outer(self):
        data = self.data
        count = self.count
        inner_count = self.inner_count
        return "{" + ", ".join([
               "{" + ", ".join([
                   str(data[i]['data'][j])
               for j in range(inner_count)]) + "}"
               for i in range(count)]) + "}"

    def children(self):
        data = self.data
        count = self.count

        if count <= 4:
            for i in range(count):
                yield (["x", "y", "z", "w"][i], data[i])
        else:
            for i in range(count):
                yield (str(i), data[i])

    def display_hint(self):
        return "array"

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
    s = str(val.type.strip_typedefs())
    if re.match(r"^tl::Array<.*>$", s):
        return ArrayPrinter(val)
    if re.match(r"^tl::Span<.*>$", s):
        return SpanPrinter(val)
    if re.match(r"^tl::List<.*>$", s):
        return ListPrinter(val)
    return None

gdb.pretty_printers.append(my_lookup_function)