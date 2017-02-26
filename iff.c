#include "iff.h"

void idToString(IffID id, gchar *str, gsize str_max)
{
	gint	i;

	g_assert(str != NULL && str_max >= 5);
	str += 4;
	*str-- = '\0';
	for(i = 4; i > 0; --i)
	{
		*str-- = id & 0xFF;
		id >>= 8;
	}
}

gboolean readUlong(FILE *file, guint32 *dest)
{
	guint32		val;
	gboolean	success;

	g_assert(file != NULL);
	success = fread(&val, sizeof val, 1, file) == 1;
	if(!success)
		g_warning("Error reading (4).");
	else
	{
		if(dest != NULL)
			*dest = GUINT32_FROM_BE(val);
	}
	return success;
}

gboolean writeUlong(FILE *file, guint32 val)
{
	gboolean	success;

	g_assert(file != NULL);
	val = GUINT32_TO_BE(val);
	success = fwrite(&val, sizeof val, 1, file) == 1;
	if(!success)
		g_warning("Error writing (4).");
	return success;
}

gboolean readUword(FILE *file, guint16 *dest)
{
	guint16		val;
	gboolean	success;

	g_assert(file != NULL);
	success = fread(&val, sizeof val, 1, file) == 1;
	if(!success)
		g_warning("Error reading (2).");
	else
	{
		if(dest != NULL)
			*dest = GUINT16_FROM_BE(val);
	}
	return success;
}

gboolean writeUword(FILE *file, guint16 val)
{
	gboolean	success;

	g_assert(file != NULL);
	val = GUINT16_TO_BE(val);
	success = fwrite(&val, sizeof val, 1, file) == 1;
	if(!success)
		g_warning("Error writing (2).");
	return success;
}

gboolean readUchar(FILE *file, guint8 *dest)
{
	int		erg;
	gboolean	success;

	g_assert(file != NULL);
	erg = fgetc(file);
	success = erg != EOF;
	if(!success)
		g_warning("Error reading (1).");
	else
	{
		if(dest != NULL)
			*dest = erg;
	}
	return success;
}

gboolean writeUchar(FILE *file, guint8 val)
{
	gboolean	success;

	g_assert(file != NULL);
	success = fputc((int) val, file) != EOF;
	if(!success)
		g_warning("Error writing (1).");
	return success;
}

gboolean writeLongAt(FILE *file, gint32 val, gint32 fileOffset)
{
	gboolean	success;

	success = fseek(file, fileOffset, SEEK_SET) != -1;
	if(success)
		success = writeUlong(file, val);
	return success;
}

static long afterChunk = 0;	/* FIXME */

gboolean iffReadHeader(FILE *file, IffChunkHeader *chd)
{
	gboolean	success = TRUE;

	g_assert(file != NULL);
	g_assert(chd != NULL);
	if(afterChunk)
		success = fseek (file, afterChunk, SEEK_SET) == 0;
	if(success)
	{
		success = readUlong(file, &chd->id);
		if(success)
		{
			success = readUlong(file, &chd->len);
			if(success)
			{
				if(ID_FORM != chd->id)	/* FIXME!! */
				{
					afterChunk = ftell(file);
					success = afterChunk != -1;
					if(!success)
					{
						afterChunk = 0;	/* To be sure */
					}
					else
					{
						afterChunk += ((chd->len + 1) & ~1UL);	/* FIXME */
					}
				}
			}
		}
	}
	else
		g_warning("Error fseek()ing.");
	return success;
}

gboolean iffWriteHeader(FILE *file, const IffChunkHeader *chd)
{
	gboolean	success;

	g_assert(file != NULL);
	g_assert(chd != NULL);
	success = writeUlong(file, chd->id);
	if(success)
		success = writeUlong(file, chd->len);
	return success;
}

void iffInitHeader(IffChunkHeader *chd, IffID id, guint32 len)
{
	g_assert(chd != NULL);
	chd->id = id;
	chd->len = len;
}

void iffDumpHeader(const IffChunkHeader *chd)
{
	gchar	idstr[5];

	g_assert(chd != NULL);
	idToString(chd->id, idstr, sizeof idstr);
	printf("Header %s of %lu bytes.\n", idstr, (unsigned long) chd->len);
}

gboolean iffReadData(FILE *file, void *data, gsize len)
{
	g_assert(file != NULL);
	g_assert(data != NULL);
	const gboolean success = fread(data, len, 1, file) == 1;
	if(!success)
		g_warning("Error reading.");
	return success;
}

gboolean iffWriteData(FILE *file, const void *data, gsize len)
{
	g_assert(file!= NULL);
	g_assert(data != NULL);
	const gboolean success = fwrite(data, len, 1, file) == 1;
	if(!success)
		g_warning("Error writing.");
	return success;
}
