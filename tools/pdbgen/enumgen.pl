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
    $destdir = '.';
}

use Text::Wrap qw(wrap $columns);
$columns = 79;

my $header = <<'HEADER';
:# The GIMP -- an image manipulation program
:# Copyright (C) 1999 Manish Singh <yosh@gimp.org>
:
:# This program is free software; you can redistribute it and/or modify
:# it under the terms of the GNU General Public License as published by
:# the Free Software Foundation; either version 2 of the License, or
:# (at your option) any later version.
:
:# This program is distributed in the hope that it will be useful,
:# but WITHOUTFILE ANY WARRANTY; without even the implied warranty of
:# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
:# GNU General Public License for more details.
:
:# You should have received a copy of the GNU General Public License
:# along with this program; if not, write to the Free Software
:# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
:
:# autogenerated by enumgen.pl
:
:package Gimp::CodeGen::enums;
:
:%enums = (
HEADER

my $footer = <<'FOOTER';
:);
:
:foreach $e (values %enums) {
:    $e->{info} = "";
:    foreach (@{$e->{symbols}}) { $e->{info} .= "$_ ($e->{mapping}->{$_}), " }
:    $e->{info} =~ s/, $//;
:}
:
:1;
FOOTER

$header =~ s/^://mg;
$footer =~ s/^://mg;

my ($enumname, $contig, $symbols, @mapping, $before);

# Most of this enum parsing stuff was swiped from makeenums.pl in GTK+
sub parse_entries {
    my $file = shift;

    while (<$file>) {
	# Read lines until we have no open comments
	while (m@/\*
	       ([^*]|\*(?!/))*$
	       @x) {
	    my $new;
	    defined ($new = <$file>) || die "Unmatched comment";
	    $_ .= $new;
	}
	# Now strip comments
	s@/\*(?!<)
	    ([^*]+|\*(?!/))*
	   \*/@@gx;
        
	s@\n@ @;
        
	next if m@^\s*$@;

	# Handle include files
	if (/^\#include\s*<([^>]*)>/ ) {
	    my $file= "../$1";
	    open NEWFILE, $file or die "Cannot open include file $file: $!\n";

	    if (&parse_entries (\*NEWFILE)) {
		return 1;
	    } else {
		next;
	    }
	}

	if (/^\s*\}\s*(\w+)/) {
	    $enumname = $1;
	    return 1;
	}

	if (m@^\s*
	      (\w+)\s*                   # name
	      (?:=(                      # value
		   (?:[^,/]|/(?!\*))*
		  ))?,?\s*
	      (?:/\*<                    # options 
		(([^*]|\*(?!/))*)
	       >\*/)?
	      \s*$
             @x) {
            my ($name, $value) = ($1, $2);

	    $symbols .= $name . ' ';

	    # Figure out a default value (not really foolproof)
	    $value = $before + 1 if !defined $value;
	    $value =~ s/\s*$//s;
	    $value =~ s/^\s*//s;

	    push @mapping, $name, $value;

	    my $test = $before + 1;

	    # Warnings in our eval should be fatal so they set $@
	    local $SIG{__WARN__} = sub { die $_[0] };

	    # Try to get a numeric value
	    eval "\$test = $value * 1;";

	    # Assume noncontiguous if it's not a number
	    $contig = 0 if $contig && ($@ || $test - 1 != $before);

	    $before = $test;
        } else {
            print STDERR "Can't understand: $_\n";
        }
    }
    return 0;
}

my $code = "";
while (<>) {
    if (eof) {
        close (ARGV);           # reset line numbering
    }

    if (/^\s*typedef\s+enum\s*({)?/) {
	# Didn't have trailing '{' look on next lines
	if (!defined $1) {
	    while (<>) {
		if (s/^\s*\{//) {
		    last;
		}
	    }
	}

	$symbols = ""; $contig = 1; $before = -1; @mapping = ();

	# Now parse the entries
	&parse_entries (\*ARGV);

	$symbols =~ s/\s*$//s;
	$symbols = wrap("\t\t\t  ", "\t\t\t  " , $symbols);
	$symbols =~ s/^\s*//s;

	my $mapping = ""; $pos = 1;
	foreach (@mapping) {
	    $mapping .= $pos++ % 2 ? "$_ => " : "'$_',\n\t\t       ";
	}
	$mapping =~ s/,\n\s*$//s;

	$code .= <<ENTRY;
    $enumname =>
	{ contig => $contig,
	  symbols => [ qw($symbols) ],
	  mapping => { $mapping }
	},
ENTRY
    }
}

$code =~ s/,\n$/\n/s;

open OUTFILE, "> $destdir/enums.pl";
print OUTFILE $header, $code, $footer;
close OUTFILE;
