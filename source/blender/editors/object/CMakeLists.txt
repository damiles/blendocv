# $Id: CMakeLists.txt 12931 2007-12-17 18:20:48Z theeth $
# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# Contributor(s): Jacques Beaurain.
#
# ***** END GPL LICENSE BLOCK *****

set(INC
	../include
	../../blenkernel
	../../blenlib
	../../blenloader
	../../gpu
	../../ikplugin
	../../imbuf
	../../makesdna
	../../makesrna
	../../python
	../../render/extern/include
	../../windowmanager
	../../../../intern/guardedalloc
)

set(INC_SYS

)

set(SRC
	object_add.c
	object_bake.c
	object_constraint.c
	object_edit.c
	object_group.c
	object_hook.c
	object_lattice.c
	object_modifier.c
	object_ops.c
	object_relations.c
	object_select.c
	object_shapekey.c
	object_transform.c
	object_vgroup.c

	object_intern.h
)

if(WITH_PYTHON)
	add_definitions(-DWITH_PYTHON)
endif()

if(WITH_GAMEENGINE)
	add_definitions(-DWITH_GAMEENGINE)
endif()

blender_add_lib(bf_editor_object "${SRC}" "${INC}" "${INC_SYS}")
