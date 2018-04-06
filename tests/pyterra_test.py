import unittest

from pyterra import pyterra

class MainTest(unittest.TestCase):
    def test_normal(self):
        tin = pyterra.PyTerra([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0], 3, 3)
        tin.do_insertion()
        res = tin.get_result()
        self.assertEqual(len(res), 2)

    def test_triangle(self):
        tin = pyterra.PyTerra([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0], 3, 3, ((0,0), (0,2), (2,0)))
        tin.do_insertion()
        res=tin.get_result()
        self.assertEqual(len(res), 1)

if __name__ == '__main__':
    unittest.main()