import random
import decimal
def generate_random_decimal(prec: int = 20):
    return random.randint(-10**prec + 1, 10**prec - 1) * (decimal.Decimal(10)**random.randint(-(prec // 5), prec // 5))

precisions = [5, 10, 20, 100, 1000]

f = open('decimal_add_test.txt', 'w')

for prec in precisions:
    decimal.getcontext().prec = prec
    for i in range(100):
        a = generate_random_decimal(prec)
        b = generate_random_decimal(prec)
        f.writelines([f'{prec} {a:{prec}f} {b:{prec}f}\n', f'{a + b:{prec}f}\n'])
f.close()

f = open('decimal_mul_test.txt', 'w')
for prec in precisions:
    decimal.getcontext().prec = prec
    for i in range(100):
        a = generate_random_decimal(prec)
        b = generate_random_decimal(prec)
        f.writelines([f'{prec} {a:{prec}f} {b:{prec}f}\n', f'{a * b:{prec}f}\n'])
f.close()

f = open('decimal_div_test.txt', 'w')
for prec in precisions:
    decimal.getcontext().prec = prec
    for i in range(100):
        a = generate_random_decimal(prec)
        b = generate_random_decimal(prec)
        while b.is_zero():
            b = generate_random_decimal(prec)
        f.writelines([f'{prec} {a:{prec}f} {b:{prec}f}\n', f'{a / b:{prec}f}\n'])
f.close()