from random import randrange
from timeit import Timer
from sys import stdout

###############################################################################
def show(L): print sum(L), '/', len(L), ':', L; return

###############################################################################
def reverse_1(L): # reverse a list in manner of CALL-BY-POSITION
   for i in range(0, len(L)): # for each i in [0, n)
      L.insert(i, L.pop()) # move the last element to position i
   return L

def reverse_1_test(n): reverse_1([ randrange(0, n) for i in range(n) ]); return

###############################################################################
def reverse_2(L): # reverse a list in manner of CALL-BY-RANK
   i, j = 0, len(L)-1 # starting from the first and the last element
   while i < j: # for each pair of symmetric elements L[i] and L[n-1-i]
      L[i], L[j] = L[j], L[i] # swap them and
      i, j = i+1, j-1 # consider the next pair
   return L

def reverse_2_test(n): reverse_2([ randrange(0, n) for i in range(n) ]); return

###############################################################################
if __name__ == '__main__':
   n = 13
   L = [ randrange(0, n) for i in range(n) ]; show(L) # create a random list of size n
   L = reverse_1(L); show(L) # reverse it using algorithm #1
   L = reverse_2(L); show(L) # reverse it using algorithm #2
   L.sort(); show(L) # and sort it for verification
   print('\n\t   i        n         T1     T1/n/n       T2       T2/n\n' + 64*'=')
   for i in range(0, 18): # for each i in [0, 18) (it slows down on typical PC for i more than 18)
      n = 2**i; print('\t2^%2d = %6d :' %(i, n)), # test the two algorithms with a list of size n = 2^i
      t = 1000*Timer('__main__.reverse_1_test(__main__.n)', 'import __main__').timeit(1); print('%8.1f %10.3f' %(t, t/n/n*1000)),
      t = 1000*Timer('__main__.reverse_2_test(__main__.n)', 'import __main__').timeit(1); print('%8.1f %10.3f' %(t, t/n))
      stdout.flush()
