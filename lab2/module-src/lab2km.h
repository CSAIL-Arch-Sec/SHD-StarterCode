#ifndef LAB2_KM_H
#define LAB2_KM_H

ssize_t lab2_victim_read(struct file *file_in, char __user *userbuf, size_t num_bytes, loff_t *offset);
ssize_t lab2_victim_write(struct file *file_in, const char __user *userbuf, size_t num_bytes, loff_t *offset);

#endif // LAB2_KM_H
