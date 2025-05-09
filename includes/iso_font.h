#ifndef _ISO_FONT_H_
#define _ISO_FONT_H_

/*
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */

/*
 * ISO Latin-1 Font
 *
 * Copyright (c) 2000
 * Ka-Ping Yee <ping@lfw.org>
 *
 * This font may be freely used for any purpose.
 */

/*
* adjusted 'A' 'V' to improve their dense appearance (ie. lightened)
* adjusted 'i' 'l' to improve their flow within a word (ie. widened)
* adjusted 'E' 'F' '#'
*/

#define ISO_CHAR_MIN    0x00
#define ISO_CHAR_MAX    0xFF
#define ISO_CHAR_WIDTH  8
#define ISO_CHAR_HEIGHT 16

// * __ DEFINITIONS ____________________________________________________________
extern unsigned char ISO_FONT[256 * 16]; 

#endif