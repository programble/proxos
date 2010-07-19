#!/bin/bash
# Copyright 2010 Curtis McEnroe <programble@gmail.com>
#
# This file is part of Proxos.
#
# Proxos is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Proxos is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Proxos.  If not, see <http://www.gnu.org/licenses/>.

for FILE in output/*; do
    echo " [ RM ] $FILE"
    rm -rf $FILE
done

echo " [ RM ] proxos.iso"
rm -f proxos.iso
