import unittest
import rasterio

from pyterra import pyterra


class BaseTest(unittest.TestCase):
    def test_normal(self):
        tin = pyterra.PyTerra([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0], 3, 3)
        tin.do_insertion()
        res = tin.get_result()
        self.assertEqual(len(res), 2)

    def test_triangle(self):
        tin = pyterra.PyTerra([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0], 3, 3, ((0, 0), (0, 2), (2, 0)))
        tin.do_insertion()
        res = tin.get_result()
        self.assertEqual(len(res), 1)

class RasterTest(unittest.TestCase):

    def test_list(self):
        dataset = rasterio.open('tests/dgm5.tif')
        tin = pyterra.PyTerra(dataset.read(1).tolist()[0], dataset.width, dataset.height)
        print(tin.get_point_count())
        tin.do_insertion()
        res = tin.get_result()
        print(tin.get_point_count())
        self.assertGreaterEqual(len(res), 1)

    def test_points(self):
        dataset = rasterio.open('tests/dgm5.tif')
        tin = pyterra.PyTerra(dataset.read(1).tolist()[0], dataset.width, dataset.height)
        print(tin.get_point_count())
        tin.do_insertion()
        res = tin.get_points()
        print(tin.get_point_count())
        print(len(res))


if __name__ == '__main__':
    unittest.main()
