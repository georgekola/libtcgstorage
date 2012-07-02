libtcgstorage
=============

C library for manipulation of storage devices that supports implementation of TCG Storage Opal or Enterprise SSC.

At the moment when this library was started, no open source solution for the target standarts exists. So purposes of this development are stated as the following:

1. Implement library that provides complete support of TCG Storage protocol on the host side for reading and manipulation os TPer parameters
2. Implement Linux-based Shadow MBR image with support of TCG Storage protocol
3. Implement set of tests according to TCG Opal Test Suite specification

Implementation is based on the next specifications of TCG:

1. [TCG Storage Architecture Core Specification, Version 2.00](http://www.trustedcomputinggroup.org/resources/tcg_storage_architecture_core_specification)
2. [Storage Work Group Storage Security Subsystem Class: Opal, Version 1.00 Final, Revision 3.00](http://www.trustedcomputinggroup.org/resources/storage_work_group_storage_security_subsystem_class_opal)
3. [Storage Work Group Storage Security Subsystem Class: Opal, Version 2.00 Final, Revision 1.00](http://www.trustedcomputinggroup.org/resources/storage_work_group_storage_security_subsystem_class_opal)
4. [Storage Work Group Storage Security Subsystem Class: Enterprise, Version 1.0, Revision 3.00](http://www.trustedcomputinggroup.org/resources/storage_work_group_storage_security_subsystem_class_enterprise_specification)
5. [TCG Storage Interface Interactions Specification Version 1.02, Revision 1.00](http://www.trustedcomputinggroup.org/resources/storage_work_group_storage_interface_interactions_specification)
6. [TCG Storage Opal Test Cases Specification Version 1.0](http://www.trustedcomputinggroup.org/resources/tcg_storage_opal_test_cases)
7. [Storage Application Note: Encrypting Drives Compliant with Opal SSC, Version 1.0, Revision 1.0](http://www.trustedcomputinggroup.org/resources/storage_application_note_encrypting_drives_compliant_with_opal_ssc)
