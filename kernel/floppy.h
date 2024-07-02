#ifndef FLOPPY_H
#define FLOPPY_H

extern int _fdc_get_drive_type(int drive);
extern int _fdc_get_cylinder_count(int drive);
extern int _fdc_get_default_track_size(int drive);

extern int _fdc_stop_drive();
extern int _fdc_select_drive(int drive);

extern int _fdc_reset();
extern int _fdc_recalibrate(int drive);
extern int _fdc_seek(int drive, int cylinder, int head);

#endif

