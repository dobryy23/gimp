/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#ifndef __TOOL_OPTIONS_DIALOG_H__
#define __TOOL_OPTIONS_DIALOG_H__


void   tool_options_dialog_new   (void);
void   tool_options_dialog_show  (void);
void   tool_options_dialog_free  (void);

void   tool_options_register     (ToolType     tool_type,
				  ToolOptions *tool_options);
void   tool_options_show         (ToolType     tool_type);
void   tool_options_hide         (ToolType     tool_type);


#endif  /*  __TOOL_OPTIONS_DIALOG_H__  */
