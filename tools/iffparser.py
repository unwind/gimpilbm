#!/usr/bin/env python
#
# Simplistic IFF chunk parser. For debugging my GIMP plug-in.
#
# Created in April 2016, by Emil Brink <emil@obsession.se>.
#

import struct
import sys


class ChunkStream(object):
	def __init__(self, data, id0 = None):
		self._data = data
		self._pos = 0
		if len(self._data) < 12:
			print "**Fail, too short string"
		form = self.get_id()
		if form != "FORM":
			print "**Fail, not IFF, missing FORM"
			return
		flen = self.get_u32()
		if id0 is not None:	# Verify IFF type.
			sid0 = self.get_id()
			if sid0 != id0:
				print "**Fail, file is '%s', not '%s'" % (sid0, id0)

	def chunks_left(self):
		return len(self._data) - self._pos >= 8

	def get_id(self):
		rid = struct.unpack(">4s", self._data[self._pos : self._pos + 4])
		self._pos += 4
		return rid[0]

	def get_u32(self):
		ru = struct.unpack(">L", self._data[self._pos : self._pos + 4])
		self._pos += 4
		return ru[0]

	def get_data(self, dlen):
		r = self._data[self._pos : self._pos + dlen]
		self._pos += dlen
		return r

	def get_chunk(self):
		cid = self.get_id()
		cln = self.get_u32()
		data = self.get_data(cln)
		return (cid, cln, data)

	def skip(self, distance):
		self._pos += distance


class IffAnalyzer(object):
	def __init__(self, fn):
		self._filename = fn
		self._data = None

	def analyze(self):
		if self._data is None:
			self._data = open(self._filename, "rb").read()
		cs = ChunkStream(self._data, "ILBM")
		while cs.chunks_left():
			here = cs.get_chunk()
			print "%4s %u" % here[:2]
			if here[0] == "ANNO":
				print "Annotation: \"%s\"" % here[2]
			elif here[0] == "BMHD":
				bmhd = struct.unpack(">HHhhBBBBHBBhh", here[2])
				print "Bitmap:", bmhd

if __name__ == "__main__":
	for a in sys.argv[1:]:
		ia = IffAnalyzer(a)
		ia.analyze()
