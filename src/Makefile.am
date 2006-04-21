# $Id$
#
# This file is part of pavucontrol.
#
# pavucontrol is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# pavucontrol is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with pavucontrol; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.

bin_PROGRAMS=pavucontrol

dist_pkgdata_DATA=pavucontrol.glade

pavucontrol_SOURCES=pavucontrol.cc

pavucontrol_LDADD=$(AM_LDADD) $(GUILIBS_LIBS) $(POLYP_LIBS)
pavucontrol_CXXFLAGS=$(AM_CXXFLAGS) $(GUILIBS_CFLAGS) $(POLYP_CFLAGS) 
pavucontrol_CXXFLAGS+=-DGLADE_FILE=\"$(pkgdatadir)/pavucontrol.glade\"
