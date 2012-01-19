#define MINRUN 3
#define MAXRUN 128
#define MAXDAT 128

#include "byterun1.h"

/**** ByteRun1 encoding/decoding ****/

static guint8 * packPutDump(guint8 *dest, guint8 nn, const guint8 *src)
{
	*dest++ = ((gint8) nn) - 1;
	memcpy(dest, src, nn);
	dest += nn;
	return dest;
}

static guint8 * packPutRun(guint8 *dest, guint8 nn, guint8 cc)
{
	*dest++ = -(((gint8) nn) - 1);
	*dest++ = cc;
	return dest;
}

gint32 packRow(guint8 *odest, const guint8 *src, guint32 width)
{
	guint8	*dest = odest;
	guint8	ch, lastch;
	enum {
		run, dump
	} mode = dump;
	gint16	nbuf = 1;
	gint16	rstart = 0;
	guint8	buf[256];              /* ? 128 ? */

	*buf = lastch = ch = *src++;
	--width;
	for(; width; --width)
	{
		buf[nbuf++] = ch = *src++;
		switch(mode)
		{
		case dump:
			if(nbuf > MAXDAT)
			{
				dest = packPutDump(dest, nbuf - 1, buf);
				*buf = ch;
				nbuf = 1;
				rstart = 0;
			}
			else if(ch == lastch)
			{
				if(nbuf - rstart >= MINRUN)
				{
					if(rstart > 0)
						dest = packPutDump(dest, rstart, buf);
					mode = run;
				}
				else if(!rstart)
					mode = run;
			}
			else
				rstart = nbuf - 1;
			break;
		case run:
			if((ch != lastch) || (nbuf - rstart > MAXRUN))
			{
				dest = packPutRun(dest, nbuf - 1 - rstart, lastch);
				*buf = ch;
				nbuf = 1;
				rstart = 0;
				mode = dump;
			}
			break;
		}
		lastch = ch;
	}

	if(mode == run)
		dest = packPutRun(dest, nbuf - rstart, lastch);
	else
		dest = packPutDump(dest, nbuf, buf);
	return dest - odest;
}

gboolean unpackRow(FILE *file, gint8 *bitlinebuf, gint32 bytesNeeded)
{
	gboolean	success = TRUE;

	while(bytesNeeded > 0)
	{
		gint	ch = fgetc(file);
		if(EOF == ch)
		{
			fputs("Got EOF while decrunching!\n", stderr);
			return 0;
		}
		ch = (gint8) ch;
		if(ch >= 0)
		{
			gint	skip = 0;

			++ch;
			if(ch > bytesNeeded)
			{
				skip = ch - bytesNeeded;
				fprintf(stderr, "Bad compr.: %u byte(s) too much in row. Skipping.\n", skip);
				ch = bytesNeeded;
			}
			if(fread(bitlinebuf, ch, 1, file) != 1)
			{
				fputs("Error reading compressed data, image might look confused.\n", stderr);
			}
			if(skip)
			{
				if(fseek(file, skip, SEEK_CUR))
				{
					fputs("Error skipping extraneous bytes in stream.\n", stderr);
				}
			}
			bitlinebuf += ch;
			bytesNeeded -= ch;
		}
		else if(ch != -128)
		{
			gint val;
			ch = -ch + 1;
			val = fgetc(file);
			if(EOF == val)
			{
				fputs("Got EOF while decrunching!\n", stderr);
				return FALSE;
			}
			else
			{
				if(ch > bytesNeeded)
				{
					fprintf(stderr, "Bad compr.: %u byte(s) too much in row.\n", ch - bytesNeeded);
					ch = bytesNeeded;
				}
				memset(bitlinebuf, val, ch);
				bitlinebuf += ch;
			}
			bytesNeeded -= ch;
		}
		else
		{
			/*fputs("unpackRow: Got -128!\n", stderr); */
			/* Amiga inc. says that -128 is "NOP". */
		}
	}
	if(bytesNeeded < 0)
		success = 0;

	return success;
}
