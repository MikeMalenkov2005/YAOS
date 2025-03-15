#ifndef ATTRIB_H
#define ATTRIB_H

#define __section(name) __attribute__((section(name)))
#define __packed        __attribute__((packed))
#define __naked         __attribute__((naked))
#define __aligned(n)    __attribute__((aligned(n)))

#endif
