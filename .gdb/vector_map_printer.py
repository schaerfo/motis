class VectorMapPrinter:
    def __init__(self, typename, val):
        self._typename = typename
        self._val = val

    def children(self):
        size = self._val['used_size_']
        for pos in range(size):
            elt = self._val['el_'][pos]
            yield ('[%d]' % pos, elt)

    def to_string(self):
        return ('%s of length %d, capacity %d'
                % (self._typename, self._val['used_size_'], self._val['allocated_size_']))

    def display_hint(self):
        return 'array'

class VecvecPrinter:
    def __init__(self, val):
        self._val = val
        self._typename = val.type
        self._key_type = val.type.template_argument(0)
        self._value_type = val.type.template_argument(1).template_argument(0)
        self._size = val['bucket_starts_']['used_size_']
        if self._size != 0:
            self._size -= 1

    def __len__(self):
        return self._size

    def to_string(self):
        return f"{self._typename}<{self._key_type}, {self._value_type}> of size {len(self)}"

    def children(self):
        if str(self._value_type) == 'char':
            yield from self._char_children()
        else:
            yield from self._bucket_children()

    def _char_children(self):
        data = self._val['data_']['el_']
        buckets = self._val['bucket_starts_']['el_']
        for pos in range(len(self)):
            curr_len = buckets[pos + 1] - buckets[pos]
            bucket_content = (data + buckets[pos]).string(length=curr_len)
            yield ('[%d]' % pos, bucket_content)

    def _bucket_children(self):
        data = self._val['data_']['el_']
        buckets = self._val['bucket_starts_']['el_']
        for pos in range(len(self)):
            curr_len = buckets[pos + 1] - buckets[pos] - 1
            bucket_type = self._value_type.array(curr_len)
            bucket_proxy = data[buckets[pos]].cast(bucket_type)
            yield ('[%d]' % pos, bucket_proxy)

    def display_hint(self):
        return 'array'


def cista_pretty_printer(val):
    if 'vecvec' in str(val.type):
        return VecvecPrinter(val)
    if 'basic_vector' in str(val.type):
        return VectorMapPrinter(val.type, val)
    else:
        return None

gdb.pretty_printers.append(cista_pretty_printer)
