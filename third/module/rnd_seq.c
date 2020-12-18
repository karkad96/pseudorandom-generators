#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/random.h>
#include <linux/string.h>

MODULE_LICENSE("GPL v2");

#define NUMS 20
#define SIZE ((1) << (13))

int a[NUMS + 1];
int m[NUMS + 1];

long long int m_lcg;
long long int a_lcg;
long long int c_lcg;

struct lcg_constants
{
	long long int m;
	long long int a;
	long long int c;
};

struct lcg_constants lcg_params[10] = 
{
	{(1LL << 32),     1664525,    1013904223},
	{(1LL << 32),     22695477,   1},
	{(1LL << 31),     1103515245, 12345},
	{(1LL << 32),     134775813,  1},
	{(1LL << 32),	  214013,     2531011},
	{(1LL << 24),     1140671485, 12820163},
	{(1LL << 31) - 1, 2147483629, 2147483587},
	{(1LL << 31) - 1, 16807,      0},
	{(1LL << 31) - 1, 48271,      0},
	{(1LL << 23),     65793,      4282663}
};

long long int get_next_lcg(void)
{
	static long long int x0 = 0;

	x0 = (a_lcg * x0 + c_lcg) % m_lcg;

	return x0;
}


int generate_random_number(void)
{
	int rnd = 0;

	get_random_bytes(&rnd, sizeof(int) - 1);

	return rnd;
}

int generate_random_bit(void)
{
	int rnd = 0;

	get_random_bytes(&rnd, sizeof(int) - 1);

	return rnd % 2;
}

unsigned int first_zero_in_binary(int n)
{
	unsigned int first_zero = 1;

	while(n % 2)
	{
		n /= 2;
		first_zero++;
	}

	return first_zero;
}

int int_length(long long int n)
{
        int length = 0;

        if(n == 0)
                return 1;

        while(n)
        {
                n /= 10;
                length++;
        }

        return length;
}

void fill_am_tables(void)
{
	int i;
	
	for(i = 1; i <= 3; i++)
	{
		a[i] = generate_random_bit();
	}

	for(i = 1; i <= 4; i++)
	{
		m[i] = generate_random_number() % (1 << i);
		
		if(m[i] % 2 == 0)
			m[i] += 1;
	}

	for(i = 5; i <= NUMS; i++)
	{
		int k;
		int j = 1;
		int xor = 0;
		int pow2 = 2;

		for(k = i - 1; k >= i - 3; k--)
		{
			xor ^= (pow2 * a[j++] * m[k]);
			pow2 *= 2;
		}

		m[i] = xor ^ (pow2 * m[k]) ^ m[k];
	}	
}

void sobol(char* output_str, int size)
{
	static int figure = 0; 
	static int cache_x_i[SIZE + 1] = {0};
	static int cache_x_i_den[SIZE + 1] = {0};

	int i;
	int x_i = 0;
	int x_i_den = 1;
	int index = generate_random_number() % (SIZE - (SIZE >> 1) + 1) + (SIZE >> 1);

	if(figure % 512 == 0)
	{
		fill_am_tables();

		for(i = 0; i <= SIZE; i++)
		{
			int fzb = first_zero_in_binary(i - 1);
			int m_i = m[fzb];

			if(x_i_den < 1 << fzb)
			{
				x_i = (x_i * ((1 << fzb) / x_i_den)) ^ m_i;
				x_i_den = 1 << fzb;
			}
		
			else
			{
				x_i = x_i ^ (m_i * (x_i_den / (1 << fzb)));	
			}
			
			cache_x_i[i] = x_i;
			cache_x_i_den[i] = x_i_den;
		}
	}

	else
	{
		x_i = cache_x_i[index];
		x_i_den = cache_x_i_den[index];
	}

	figure++;

	output_str[0] = '0';
	output_str[1] = '.';

	for(i = 2; i < size + 2; i++)
	{
		output_str[i] = (char)((x_i * 10) / x_i_den + 48);
		x_i = (x_i * 10) % x_i_den;
	}

	output_str[i] = '\0';
}

void halton(char* output_str, int base, int size)
{
        int f_den = 1;
        int r_num = 0;
        int r_den = 1;

	int i = generate_random_number() % (SIZE - (SIZE >> 1) + 1) + (SIZE >> 1);

        while(i)
        {
                f_den *= base;

                r_num = r_num * (f_den / r_den) + i % base;
                r_den = f_den;

                i /= base;
        }

 	output_str[0] = '0';
	output_str[1] = '.';

        for(i = 2; i < size + 2; i++)
        {
                output_str[i] = (char)((r_num * 10) / f_den + 48);
                r_num = (r_num * 10) % f_den;
        }

        output_str[i] = '\0';
}

void LCG(char* output_str, int size)
{
	static int figure = 0;
	
        int i;
        int length;
	long long int tmp;
	
	if(figure % (1 << 25) == 0)
	{
		int index = generate_random_number() % 10;

        	m_lcg = lcg_params[index].m;
        	a_lcg = lcg_params[index].a;
		c_lcg = lcg_params[index].c;
	
		tmp = generate_random_number() % (1 << 31) + 1;	
	}
	
	else
	{
		tmp = get_next_lcg();
	}

	figure++;	
	length = int_length(tmp);

        for(i = length - 1; i >= 0; i--)
        {
                output_str[i] = (char)((tmp % 10) + 48);
                tmp /= 10;
        }
	
	for(i = length; i < size; i++)
	{
		output_str[i] = (char)(48);
	}

        output_str[size] = '\0';
}

EXPORT_SYMBOL(sobol); 
EXPORT_SYMBOL(halton);
EXPORT_SYMBOL(LCG);
