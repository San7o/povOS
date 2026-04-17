# Resources

Legacy documents (still very useful):
- [MultiProcessor
Specification](https://web.archive.org/web/20121002210153/http://download.intel.com/design/archives/processors/pro/docs/24201606.pdf)
describes a common architecture for the PC (replaced by ACPI)
- [PIIX3
  Datasheet](https://www.alldatasheet.com/datasheet-pdf/pdf/66093/INTEL/PIIX3.html):
  the old southbridge (replaced by PCH, which is retro-compatible),
  allows accesses to a lot of devices including the ancient (but very
  easy to work with) ISA-based devices like PIT and PIC, and "newer"
  toys like IOAPIC.

Modern documents:

- [ACPI Specification (6.6)](https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/): standard for
  "Plug and Play" devices and their resources. The specification
  provides the structure of all types of tables.
- [Intel I/O Controller Hub 9 (ICH9)
  Family](https://www.intel.com/content/dam/doc/datasheet/io-controller-hub-9-datasheet.pdf):
  modern southbridge, providing a massive amount of
  functionality. This is qemu's `q32-9.0` emulated machine.
- [PCIe Specification (revision 6.0)](https://github.com/Tvirus/ebook/blob/main/ISO/PCI/PCI%20Express%20Base%20Specification%20Revision%206.0.pdf): one of the major buses on the PC
- [Intel 64 and IA-32 Architectures Software Developer’s
  Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html):
  one of the most important books to master, it explains all CPU
  functionalities like paging, segmentation, virtualization and the
  rest.
- [Multiboot Specification version 0.6.96](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html): standard interface implemented by bootloaders

Useful theory:

- [ CppCon 2017: Fedor Pikus "Read, Copy, Update, then what? RCU for
  non-kernel
  programmers"](https://www.youtube.com/watch?v=rxQ5K9lo034): explains
  RCU, which is very useful for read-mostly data structures
