/*
 * nOSGi Project - http://www-vs.informatik.uni-ulm.de/proj/nosgi/
 *
 * Copyright (C) 2009-2011 Aspectix Research Team,
 * Institute of Distributed Systems, Ulm University, Germany
 *
 * This file is part of the nOSGi Project, which is released
 * under the terms of the GNU General Public License.
 * See the LICENSE.txt file in the root directory of the software
 * distribution for more details.
 */

#include <nosgi/classloader/solib.h>
#include <fcntl.h>
#include <gelf.h>
#include <libelf.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

using namespace std;

static int getDynamicStringTableIndex(Elf *elf) {
	Elf_Scn *scn = NULL;
	GElf_Shdr shdr;
	while((scn=elf_nextscn(elf, scn)) != NULL) {
		gelf_getshdr(scn, &shdr);
		if(shdr.sh_type==SHT_DYNAMIC) {
			return (int)shdr.sh_link;
		}
	}
	return -1;
}

int replaceSymbols(const char *filename, map<string,string> &symbols) {
	int fd;
	Elf *elf;
	Elf_Scn *scn = NULL;
	Elf_Data *edata = NULL;
	int update = 0;

	if(elf_version(EV_CURRENT) == EV_NONE) {
		fprintf(stderr, "Elf Library is out of date!\n");
		return -1;
	}
	if((fd=open(filename, O_RDWR))<0) {
		fprintf(stderr, "error opening %s: ", filename);
		perror("open");
		return -1;
	}
	if((elf=elf_begin(fd, ELF_C_RDWR, NULL)) == NULL) {
		fprintf(stderr, "elf_begin(): %s\n", elf_errmsg(0));
		close(fd);
		return -1;
	}

	scn=elf_getscn(elf, getDynamicStringTableIndex(elf));
	if(!(edata=elf_getdata(scn, NULL))) {
		fprintf(stderr, "can't read string table\n");
		elf_end(elf);
		close(fd);
		return -1;
	}
	for (unsigned int i=0; i<edata->d_size; i++) {
		if (symbols.count(((char *)edata->d_buf)+i)>0) {
			string &symbol(symbols[((char *)edata->d_buf)+i]);
			memcpy(((char *)edata->d_buf)+i, symbol.c_str(),
					symbol.length());
			elf_flagdata(edata, ELF_C_SET, ELF_F_DIRTY);
			update = 1;
		}
		while(*(((char *)edata->d_buf)+i)!=0 && i<edata->d_size) {
			i++;
		}
	}
	if (update) {
		//Layout nicht reparieren
		elf_flagelf(elf, ELF_C_SET, ELF_F_LAYOUT);
		//Update durchführen
		if (elf_update(elf, ELF_C_WRITE) < 0) {
			printf("elf_update failed: %s\n", elf_errmsg(0));
			return -1;
		}
	}
	elf_end(elf);
	close(fd);
	return 0;
}
