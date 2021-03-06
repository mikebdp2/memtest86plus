#ifndef COREBOOT_TABLES_H
#define COREBOOT_TABLES_H

#include "stdint.h"

/* The coreboot table information is for conveying information
 * from the firmware to the loaded OS image.  Primarily this
 * is expected to be information that cannot be discovered by
 * other means, such as querying the hardware directly.
 *
 * All of the information should be Position Independent Data.
 * That is it should be safe to relocated any of the information
 * without it's meaning/correctness changing.   For table that
 * can reasonably be used on multiple architectures the data
 * size should be fixed.  This should ease the transition between
 * 32 bit and 64 bit architectures etc.
 *
 * The completeness test for the information in this table is:
 * - Can all of the hardware be detected?
 * - Are the per motherboard constants available?
 * - Is there enough to allow a kernel to run that was written before
 *   a particular motherboard is constructed? (Assuming the kernel
 *   has drivers for all of the hardware but it does not have
 *   assumptions on how the hardware is connected together).
 *
 * With this test it should be straight forward to determine if a
 * table entry is required or not.  This should remove much of the
 * long term compatibility burden as table entries which are
 * irrelevant or have been replaced by better alternatives may be
 * dropped.  Of course it is polite and expedite to include extra
 * table entries and be backwards compatible, but it is not required.
 */


struct lb_header
{
	uint8_t  signature[4]; /* LBIO */
	uint32_t header_bytes;
	uint32_t header_checksum;
	uint32_t table_bytes;
	uint32_t table_checksum;
	uint32_t table_entries;
};

/* Every entry in the boot environment list will correspond to a boot
 * info record.  Encoding both type and size.  The type is obviously
 * so you can tell what it is.  The size allows you to skip that
 * boot environment record if you don't know what it is.  This allows
 * forward compatibility with records not yet defined.
 */
struct lb_record {
	uint32_t tag;		/* tag ID */
	uint32_t size;		/* size of record (in bytes) */
};

#define LB_TAG_UNUSED	0x0000

#define LB_TAG_MEMORY	0x0001
#define LB_TAG_SERIAL	0x000f
#define LB_TAG_FORWARD	0x0011

struct lb_memory_range {
	uint64_t start;
	uint64_t size;
	uint32_t type;
#define LB_MEM_RAM		 1	/* Memory anyone can use */
#define LB_MEM_RESERVED		 2	/* Don't use this memory region */
#define LB_MEM_ACPI		 3	/* ACPI Tables */
#define LB_MEM_NVS		 4	/* ACPI NVS Memory */
#define LB_MEM_UNUSABLE		 5	/* Unusable address space */
#define LB_MEM_VENDOR_RSVD	 6	/* Vendor Reserved */
#define LB_MEM_TABLE		16	/* Ram configuration tables are kept in */
};

struct lb_memory {
	uint32_t tag;
	uint32_t size;
	struct lb_memory_range map[0];
};

#define LB_SERIAL_TYPE_IO_MAPPED     1
#define LB_SERIAL_TYPE_MEMORY_MAPPED 2

struct lb_serial {
	uint32_t tag;
	uint32_t size;
	uint32_t type;
	uint32_t baseaddr;
	uint32_t baud;
	uint32_t regwidth;
	uint32_t input_hertz;
	uint32_t uart_pci_addr;
};

struct lb_forward {
	uint32_t tag;
	uint32_t size;
	uint64_t forward;
};

#endif
