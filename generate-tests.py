from random import randrange

f = open('add_test.txt', 'w')
limit = 10**2000
for i in range(1_000):
    a = randrange(-limit, limit)
    b = randrange(-limit, limit)
    f.write(f'{a} {b}\n{a + b}\n')
f.close()

f = open('mul_test.txt', 'w')
limit = 10**1000
for i in range(1_000):
    a = randrange(-limit, limit)
    b = randrange(-limit, limit)
    f.write(f'{a} {b}\n{a * b}\n')

f = open('div_test.txt', 'w')
limits = [10**3, 10**10, 10**50, 10**1000]
for limit in limits:
    for i in range(500):
        a = randrange(-limit, limit)
        b = randrange(-limit, limit)
        while b == 0:
            b = randrange(-limit, limit)
        f.write(f'{a} {b}\n')
        if a == 0:
            f.write(f'0 0\n')
        elif a > 0:
            if b > 0:
                f.write(f'{a // b} {a % b}\n')
            else:
                f.write(f'{-(a // (-b))} {a % (-b)}\n')
        else:
            if b > 0:
                f.write(f'{-((-a) // b) - 1} {b - ((-a) % b)}\n')
            else:
                f.write(f'{((-a) // (-b)) + 1} {-b - ((-a) % (-b))}\n')