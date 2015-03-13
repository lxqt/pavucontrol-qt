# This file is part of pavucontrol.
#
# Copyright 2006-2008 Lennart Poettering
#
# pavucontrol is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 2 of the
# License, or (at your option) any later version.
#
# pavucontrol is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with pavucontrol. If not, see
# <http://www.gnu.org/licenses/>.

dist_html_DATA = README.html style.css
EXTRA_DIST = README.html.in

MAINTAINERCLEANFILES = README.html
CLEANFILES =

if USE_LYNX
dist_doc_DATA = README

README: README.html
	lynx --dump $^ | sed 's,file://.*/doc/README.html,README,' > $@

MAINTAINERCLEANFILES += README
CLEANFILES += README
endif

tidy: README.html
	tidy -e < README.html

.PHONY: tidy
