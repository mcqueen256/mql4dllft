import unittest
from decoder import get_mql_function

class TestSimpleFunction(unittest.TestCase):
  @classmethod
  def setUpClass(cls):
    filename = 'account.accountfreemargincheck.html'
    with open('api/' + filename, 'r') as fout:
      cls.function = get_mql_function(fout.read(), filename)

  @classmethod
  def tearDownClass(cls):
      pass

class TestSimpleFunctionDecoding(TestSimpleFunction):
  def test_name(self):
    self.assertEqual(self.function.getName(), 'AccountFreeMarginCheck')

  def test_return_type(self):
    self.assertEqual(self.function.getReturnType(), 'double')

  def test_Parameters(self):
    params = self.function.getParameters()
    self.assertTrue(len(params) == 3)
    self.assertTrue(params[0][0] == 'string')
    self.assertTrue(params[0][1] == 'symbol')
    self.assertTrue(params[0][2] is None)
    self.assertTrue(params[0][3] == 'symbol')
    self.assertTrue(params[1][0] == 'int')
    self.assertTrue(params[1][1] == 'cmd')
    self.assertTrue(params[1][2] is None)
    self.assertTrue(params[1][3] == 'trade operation')
    self.assertTrue(params[2][0] == 'double')
    self.assertTrue(params[2][1] == 'volume')
    self.assertTrue(params[2][2] is None)
    self.assertTrue(params[2][3] == 'volume')

  def test_commnet(self):
    comment = 'Returns free margin that remains after the specified order has been opened at the current price on the current account.'
    self.assertEqual(self.function.getComment(), comment)

  def test_return_comment(self):
    comment = 'Free margin that remains after the specified order has been opened at the current price on the current account. If the free margin is insufficient, an error 134 (ERR_NOT_ENOUGH_MONEY) will be generated.'
    self.assertEqual(self.function.getReturnComment(), comment)

  def test_note(self):
    note = 'Returns free margin that remains after the specified order has been opened at the current price on the current account.'
    self.assertEqual(self.function.getNote(), note)

  def test_namespace(self):
    self.assertEqual(self.function.getNamespace(), 'account')
