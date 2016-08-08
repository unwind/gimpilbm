#ifndef iff_h
#define iff_h

#include <stdio.h>
#include <glib.h>

#define MAKE_ID(a,b,c,d) ((IffID) ((a)<<24 | (b)<<16 | (c)<<8 | (d)))

#define ID_FORM	MAKE_ID('F','O','R','M')
#define ID_LIST	MAKE_ID('L','I','S','T')
#define ID_CAT_	MAKE_ID('C','A','T',' ')
#define ID_PROP	MAKE_ID('P','R','O','P')
#define ID_____	MAKE_ID(' ',' ',' ',' ')
#define ID_END_	MAKE_ID('E','N','D',' ')
#define ID_IFF_	MAKE_ID('I','F','F',' ')
#define ID_TEXT	MAKE_ID('T','E','X','T')

typedef guint32 IffID;

typedef struct {
	IffID	id;
	guint32	len;
} IffChunkHeader;

typedef struct {
	IffChunkHeader	hdr;
	FILE*		file;
	long		offs;
	guint32		undone;
} IffChunkStream;

typedef struct {
	FILE* file;
} IffHandle;

extern void	idToString(IffID id, gchar *str, gsize str_max);
extern gboolean	readUlong(FILE *file, guint32 *dest);
extern gboolean	writeUlong(FILE *file, guint32 val);
extern gboolean	readUword(FILE *file, guint16 *dest);
extern gboolean	writeUword(FILE *file, guint16 val);
extern gboolean	readUchar(FILE *file, guint8 *dest);
extern gboolean	writeUchar(FILE *file, guint8 val);
extern gboolean	writeLongAt(FILE *file, gint32 val, gint32 fileOffset);
extern gboolean	iffReadHeader(FILE *file, IffChunkHeader *chd);
extern gboolean	iffWriteHeader(FILE *file, const IffChunkHeader *chd);
extern void	iffInitHeader(IffChunkHeader *chd, IffID id, guint32 len);
extern void	iffDumpHeader(const IffChunkHeader *chd);
extern gboolean	iffReadData(FILE *file, void *data, gsize len);
extern gboolean	iffWriteData(FILE *file, const void *data, gsize len);

#endif
