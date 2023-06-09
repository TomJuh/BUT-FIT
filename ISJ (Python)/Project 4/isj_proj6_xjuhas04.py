# usr/bin/env python3


class Polynomial:
    def __init__(self, *args, **kwargs):
        # Input is just a list
        if args and isinstance(args[0], list):
            self.poly = args[0]
        elif args:   # input is just list of elements
            self.poly = args
        else:   # input is direct
            maxKey = 0   # finds the size of array, based on max key
            for key, value in kwargs.items():
                if maxKey < int(''.join(filter(str.isdigit, key))):
                    maxKey = int(''.join(filter(str.isdigit, key)))
            self.poly = [
                kwargs.get('x' + str(i), 0) for i in range(maxKey + 1)
            ]   # create list and fill it with 0 by default
            for (
                key,
                value,
            ) in (
                kwargs.items()
            ):   # fills list with values and keys from kwargs
                key = int(''.join(filter(str.isdigit, key)))
                self.poly[key] = value

    # Create string out of poly
    def __str__(self):
        result_str = list(self.poly)
        result_str.reverse()   # reverse list

        for i in range(len(result_str)):
            if int(result_str[i]) > 0:   # add + if positive
                result_str[i] = (
                    '+'
                    + str(result_str[i])
                    + 'x^'
                    + str(len(result_str) - (i + 1))
                )
            elif int(result_str[i]) < 0:   # add - if negative
                result_str[i] = (
                    str(result_str[i]) + 'x^' + str(len(result_str) - (i + 1))
                )
            elif int(result_str[i]) == 0:   # remove 0
                result_str[i] = ''

        result_str.reverse()
        for i in range(len(result_str)):
            result_str[i] = str(result_str[i])
            if result_str[i] == '0':
                result_str[i] = ''
            if 'x^0' in result_str[i]:
                result_str[i] = result_str[i].replace('x^0', '')
            if 'x^1' in result_str[i]:
                result_str[i] = result_str[i].replace('^1', '')
        while '' in result_str:
            result_str.remove('')
        if len(result_str) > 0:
            result_str[-1] = '' + result_str[-1][1:]
        elif len(result_str) == 0:
            return '0'

        return (
            ''.join(result_str[::-1])
            .replace('+', ' + ')
            .replace('-', ' - ')
            .replace('1x', 'x')
        )

    # Compare all elements of both lists
    def __eq__(self, other):
        for i in range(len(self.poly)):
            return self.poly[i] == other.poly[i]

    # Calculate the power of poly
    def __pow__(self, other):
        if other == 0:   # x^0 = 1
            return Polynomial([1])
        elif other == 1:   # x^1 = x
            return self
        else:   # x^n = x * x^(n-1)
            return self * self ** (other - 1)

    # Calculate the sum of poly
    def __add__(self, other):
        result = []
        for i in range(max(len(self.poly), len(other.poly))):
            if i < len(self.poly):   # fix for diffrent length lists
                result.append(self.poly[i])
            else:
                result.append(0)
            if i < len(other.poly):
                result[i] += other.poly[i]   # add to index
        return Polynomial(result)

    # Calculate the mul of poly
    def __mul__(self, other):
        result = []
        for i in range(len(self.poly) + len(other.poly) - 1):
            result.append(0)   # create list of 0
        for i in range(len(self.poly)):   # multiply through nested for
            for j in range(len(other.poly)):
                result[i + j] += self.poly[i] * other.poly[j]
        return Polynomial(result)

    # get value for x and y of poly
    def at_value(self, x, y=None):
        result = 0
        for i in range(len(self.poly)):   # calc poly for x
            result += self.poly[i] * x**i
        if y is not None:   # calc poly for y
            resulty = 0
            for i in range(len(self.poly)):
                resulty += self.poly[i] * y**i
            return resulty - result   # return sub

        return result

    # calculate derivation
    def derivative(self):
        result = []
        """
        list goes from lowest value to largest
        poly elements can be multipled by index since index
        also indicates the power of x
        """
        for i in range(1, len(self.poly)):
            result.append(i * self.poly[i])
        return Polynomial(result)


def test():
    assert (
        str(Polynomial(0, 1, 0, -1, 4, -2, 0, 1, 3, 0))
        == '3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x'
    )
    assert (
        str(Polynomial([-5, 1, 0, -1, 4, -2, 0, 1, 3, 0]))
        == '3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5'
    )
    assert (
        str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3=-1, x1=1))
        == '3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x'
    )
    assert str(Polynomial(x2=0)) == '0'
    assert str(Polynomial(x0=0)) == '0'
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2, 0, 3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1) + Polynomial(x1=1)) == 'x + 1'
    assert str(Polynomial([-1, 1, 1, 0]) + Polynomial(1, -1, 1)) == '2x^2'
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1 + pol2) == '3x^2 + x + 1'
    assert str(pol1 + pol2) == '3x^2 + x + 1'
    assert str(Polynomial(x0=-1, x1=1) ** 1) == 'x - 1'
    assert str(Polynomial(x0=-1, x1=1) ** 2) == 'x^2 - 2x + 1'
    pol3 = Polynomial(x0=-1, x1=1)
    assert str(pol3**4) == 'x^4 - 4x^3 + 6x^2 - 4x + 1'
    assert str(pol3**4) == 'x^4 - 4x^3 + 6x^2 - 4x + 1'
    assert str(Polynomial(x0=2).derivative()) == '0'
    assert str(Polynomial(x3=2, x1=3, x0=2).derivative()) == '6x^2 + 3'
    assert str(Polynomial(x3=2, x1=3, x0=2).derivative().derivative()) == '12x'
    pol4 = Polynomial(x3=2, x1=3, x0=2)
    assert str(pol4.derivative()) == '6x^2 + 3'
    assert str(pol4.derivative()) == '6x^2 + 3'
    assert Polynomial(-2, 3, 4, -5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3, 5) == 44
    pol5 = Polynomial([1, 0, -2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1, 3.6) == -23.92
    assert pol5.at_value(-1, 3.6) == -23.92


if __name__ == '__main__':
    test()
