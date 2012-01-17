#ifndef iff_h
#define iff_h

#include <stdio.h>
#include <netinet/in.h>
#include <glib.h>

#define MAKE_ID(a,b,c,d) ((IffID) ((a)<<24 | (b)<<16 | (c)<<8 | (d)))
#define MAKE_IDNL(l)	 ((IffID) ntohl(l))

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
  IffID id;
  guint32 len;
} IffChunkHeader;

typedef struct {
  IffChunkHeader hdr;
  FILE* fil;
  long offs;
  guint32 undone;
} IffChunkStream;

typedef struct {
  FILE* fil;
} IffHandle;

void idToString(IffID id, char *str);
gboolean readUlong(FILE * fil, guint32 * dest);
gboolean writeUlong(FILE * fil, guint32 val);
gboolean readUword(FILE * fil, guint16 * dest);
gboolean writeUword(FILE * fil, guint16 val);
gboolean readUchar(FILE * fil, guint8 * dest);
gboolean writeUchar(FILE * fil, guint8 val);
gboolean writeLongAt(FILE * fil, gint32 val, gint32 fileOffset);
gboolean iffReadHeader(FILE * fil, IffChunkHeader * chd);
gboolean iffWriteHeader(FILE * fil, const IffChunkHeader * chd);
void iffInitHeader(IffChunkHeader * chd, IffID id, guint32 len);
void iffDumpHeader(const IffChunkHeader * chd);
gboolean iffReadData(FILE * fil, void *data, size_t len);
gboolean iffWriteData(FILE * fil, const void *data, size_t len);

#define iffReadDataAuto(f,d) iffReadData((f),&(d),sizeof(d))
#define iffWriteDataAuto(f,d) iffWriteData((f),&(d),sizeof(d))

#endif
