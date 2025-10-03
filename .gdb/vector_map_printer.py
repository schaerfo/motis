class VectorMapPrinter:
    class _iterator():
        def __init__(self, start, size):
            self._item = start
            self._size = size
            self._pos = 0

        def __iter__(self):
            return self

        def __next__(self):
            if self._pos == self._size:
                raise StopIteration
            elt = self._item[self._pos]
            result = ('[%d]' % self._pos, elt)
            self._pos += 1
            return result

    def __init__(self, typename, val):
        self._typename = typename
        self._val = val

    def children(self):
        return self._iterator(self._val['el_'],
                              self._val['used_size_'])

    def to_string(self):
        return ('%s of length %d, capacity %d'
                % (self._typename, self._val['used_size_'], self._val['allocated_size_']))

    def display_hint(self):
        return 'array'

def vector_map_printer(val):
  if 'basic_vector' in str(val.type):
    return VectorMapPrinter(val.type, val)
  else:
    return None

gdb.pretty_printers.append(vector_map_printer)
