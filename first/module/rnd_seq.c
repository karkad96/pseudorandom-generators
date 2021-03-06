#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/random.h>
#include <linux/string.h>

MODULE_LICENSE("GPL v2");

#define NUMS 20
#define SIZE (1 << NUMS)

int a[NUMS + 1];
int m[NUMS + 1];

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

int int_length(int n)
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

void sobol(char* output_str)
{
	int i;
	int x_i = 0;
	int x_i_den = 1;
	int count = generate_random_number() % (1 << 10) + (1 << 10) + 1;

	fill_am_tables();

	for(i = 0; i < count; i++)
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
	}

	output_str[0] = '0';
	output_str[1] = '.';

	for(i = 2; i < NUMS + 2 && x_i; i++)
	{
		output_str[i] = (char)((x_i * 10) / x_i_den + 48);
		x_i = (x_i * 10) % x_i_den;
	}

	output_str[i] = '\0';
}

void halton(char* output_str, int base)
{
        int f_den = 1;
        int r_num = 0;
        int r_den = 1;
        int count = 0;
        int i = generate_random_number() % (1 << 10) + (1 << 10) + 1;

        while(i)
        {
                f_den *= base;

                r_num = r_num * (f_den / r_den) + i % base;
                r_den = f_den;

                i /= base;
                count++;
        }

 	output_str[0] = '0';
	output_str[1] = '.';

        for(i = 2; i < count + 2; i++)
        {
                output_str[i] = (char)((r_num * 10) / f_den + 48);
                r_num = (r_num * 10) % f_den;
        }

        output_str[count] = '\0';
}

void LCG(char* output_str)
{
        int m = generate_random_number()  % (1 << 15) + (1 << 10);
        int a = generate_random_number()  % (1 << 15) + 1;
        int c = generate_random_number()  % (1 << 15) + 1;
        int x0 = generate_random_number() % (1 << 15) + 1;

        int count = generate_random_number() % (1 << 10) + (1 << 10) + 1;

        int i;
        int length;

       	for(i = 0; i < count; i++)
       	{
               	x0 = (a * x0 + c) % m;
	}

	length = int_length(x0);

        for(i = length - 1; i >= 0; i--)
        {
                output_str[i] = (char)((x0 % 10) + 48);
                x0 /= 10;
        }

        output_str[length] = '\0';
}

EXPORT_SYMBOL(sobol); 
EXPORT_SYMBOL(halton);
EXPORT_SYMBOL(LCG);
