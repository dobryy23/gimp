#!/usr/bin/perl -w

# The GIMP -- an image manipulation program
# Copyright (C) 1999 Manish Singh <yosh@gimp.org>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUTFILE ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

BEGIN {
    $srcdir  = $ENV{srcdir}  || '.';
    $destdir = $ENV{destdir} || '.';
}

use lib $srcdir;

require 'enums.pl';
require 'util.pl';

*enums = \%Gimp::CodeGen::enums::enums;

*write_file = \&Gimp::CodeGen::util::write_file;
*FILE_EXT   = \$Gimp::CodeGen::util::FILE_EXT;

my $constfile = "$destdir/script-fu-constants.c$FILE_EXT";
open CONSTFILE, "> $constfile" or die "Can't open $constfile: $!\n";

print CONSTFILE <<'GPL';
/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-1999 Spencer Kimball and Peter Mattis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* NOTE: This file is autogenerated by script-fu.pl. */

GPL

print CONSTFILE <<CODE;
#include "siod.h"

void
init_generated_constants (void)
{
CODE

foreach (sort keys %enums) {
    my $enum = $enums{$_};
    foreach $symbol (@{$enum->{symbols}}) {
	my $sym = $symbol;
	$sym = $enum->{nicks}->{$sym} if exists $enum->{nicks}->{$sym};
	$sym =~ s/_/-/g;

	print CONSTFILE <<CODE;
  setvar (cintern ("$sym"), flocons ($enum->{mapping}->{$symbol}), NIL);
CODE
    }
    print CONSTFILE "\n";
}

print CONSTFILE "  return;\n}\n";

close CONSTFILE;
&write_file($constfile);
