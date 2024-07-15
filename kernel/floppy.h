#ifndef FLOPPY_H
#define FLOPPY_H

#include <stddef.h>

extern int _fdc_get_drive_type(int drive);
extern int _fdc_get_cylinder_count(int drive);
extern int _fdc_get_default_track_size(int drive);

extern int _fdc_stop_drive();
extern int _fdc_select_drive(int drive);

extern int _fdc_reset();
extern int _fdc_recalibrate(int drive);
extern int _fdc_seek(int drive, int cylinder, int head);

// _fdc_load and _fdc_store return positive on success and negative on failure
// _fdc_load and _fdc_store will call _fdc_seek internaly
extern int _fdc_load(int drive, int cylinder, int head, int sector, int track_size);
extern int _fdc_store(int drive, int cylinder, int head, int sector, int track_size);

extern void* _fdc_get_buffer();
extern size_t _fdc_get_buffer_size();

#endif

