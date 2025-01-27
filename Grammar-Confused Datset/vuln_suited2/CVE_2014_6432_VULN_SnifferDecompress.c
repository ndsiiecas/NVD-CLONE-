static int
CVE_2014_6432_VULN_SnifferDecompress(unsigned char *inbuf, size_t inlen, unsigned char *outbuf,
		  size_t outlen, int *err)
{
	unsigned char * pin  = inbuf;
	unsigned char * pout = outbuf;
	unsigned char * pin_end  = pin + inlen;
	unsigned char * pout_end = pout + outlen;
	unsigned int bit_mask;      /* one bit is set in this, to mask with bit_value */
	unsigned int bit_value = 0; /* cache the last 16 coding bits we retrieved */
	unsigned int code_type;     /* encoding type, from high 4 bits of byte */
	unsigned int code_low;      /* other 4 bits from encoding byte */
	int length;		    /* length of RLE sequence or repeated string */
	int offset;		    /* offset of string to repeat */

	if (inlen > G_MAXUINT16) {
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
		return ( -1 );
	}

	bit_mask  = 0;  /* don't have any bits yet */
	while (1)
	{
		/* Shift down the bit mask we use to see whats encoded */
		bit_mask = bit_mask >> 1;

		/* If there are no bits left, time to get another 16 bits */
		if ( 0 == bit_mask )
		{
			bit_mask  = 0x8000;  /* start with the high bit */
			bit_value = pletoh16(pin);   /* get the next 16 bits */
			pin += 2;          /* skip over what we just grabbed */
			if ( pin >= pin_end )
			{
				*err = WTAP_ERR_UNC_TRUNCATED;	 /* data was oddly truncated */
				return ( -1 );
			}
		}

		/* Use the bits in bit_value to see what's encoded and what is raw data */
		if ( !(bit_mask & bit_value) )
		{
			/* bit not set - raw byte we just copy */
			*(pout++) = *(pin++);
		}
		else
		{
			/* bit set - next item is encoded.  Peel off high nybble
			   of next byte to see the encoding type.  Set aside low
			   nybble while we are at it */
			code_type = (unsigned int) ((*pin) >> 4 ) & 0xF;
			code_low  = (unsigned int) ((*pin) & 0xF );
			pin++;   /* increment over the code byte we just retrieved */
			if ( pin >= pin_end )
			{
				*err = WTAP_ERR_UNC_TRUNCATED;	 /* data was oddly truncated */
				return ( -1 );
			}

			/* Based on the code type, decode the compressed string */
			switch ( code_type )
			{
			case 0  :   /* RLE short runs */
				/*
				  Run length is the low nybble of the first code byte.
				  Byte to repeat immediately follows.
				  Total code size: 2 bytes.
				*/
				length = code_low + 3;
				/* If length would put us past end of output, avoid overflow */
				if ( pout + length > pout_end )
				{
					*err = WTAP_ERR_UNC_OVERFLOW;
					return ( -1 );
				}

				/* generate the repeated series of bytes */
				memset( pout, *pin++, length );
				pout += length;
				break;
			case 1  :   /* RLE long runs */
				/*
				  Low 4 bits of run length is the low nybble of the
				  first code byte, upper 8 bits of run length is in
				  the next byte.
				  Byte to repeat immediately follows.
				  Total code size: 3 bytes.
				*/
				length = code_low + ((unsigned int)(*pin++) << 4) + 19;
				/* If we are already at end of input, there is no byte
				   to repeat */
				if ( pin >= pin_end )
				{
					*err = WTAP_ERR_UNC_TRUNCATED;	 /* data was oddly truncated */
					return ( -1 );
				}
				/* If length would put us past end of output, avoid overflow */
				if ( pout + length > pout_end )
				{
					*err = WTAP_ERR_UNC_OVERFLOW;
					return ( -1 );
				}

				/* generate the repeated series of bytes */
				memset( pout, *pin++, length );
				pout += length;
				break;
			case 2  :   /* LZ77 long strings */
				/*
				  Low 4 bits of offset to string is the low nybble of the
				  first code byte, upper 8 bits of offset is in
				  the next byte.
				  Length of string immediately follows.
				  Total code size: 3 bytes.
				*/
				offset = code_low + ((unsigned int)(*pin++) << 4) + 3;
				/* If we are already at end of input, there is no byte
				   to repeat */
				if ( pin >= pin_end )
				{
					*err = WTAP_ERR_UNC_TRUNCATED;	 /* data was oddly truncated */
					return ( -1 );
				}
				/* Check if offset would put us back past begin of buffer */
				if ( pout - offset < outbuf )
				{
					*err = WTAP_ERR_UNC_BAD_OFFSET;
					return ( -1 );
				}

				/* get length from next byte, make sure it won't overrun buf */
				length = (unsigned int)(*pin++) + 16;
				if ( pout + length > pout_end )
				{
					*err = WTAP_ERR_UNC_OVERFLOW;
					return ( -1 );
				}

				/* Copy the string from previous text to output position,
				   advance output pointer */
				memcpy( pout, pout - offset, length );
				pout += length;
				break;
			default :   /* (3 to 15): LZ77 short strings */
				/*
				  Low 4 bits of offset to string is the low nybble of the
				  first code byte, upper 8 bits of offset is in
				  the next byte.
				  Length of string to repeat is overloaded into code_type.
				  Total code size: 2 bytes.
				*/
				offset = code_low + ((unsigned int)(*pin++) << 4) + 3;
				/* Check if offset would put us back past begin of buffer */
				if ( pout - offset < outbuf )
				{
					*err = WTAP_ERR_UNC_BAD_OFFSET;
					return ( -1 );
				}

				/* get length from code_type, make sure it won't overrun buf */
				length = code_type;
				if ( pout + length > pout_end )
				{
					*err = WTAP_ERR_UNC_OVERFLOW;
					return ( -1 );
				}

				/* Copy the string from previous text to output position,
				   advance output pointer */
				memcpy( pout, pout - offset, length );
				pout += length;
				break;
			}
		}

		/* If we've consumed all the input, we are done */
		if ( pin >= pin_end )
			break;
	}

	return (int) ( pout - outbuf );  /* return length of expanded text */
}
