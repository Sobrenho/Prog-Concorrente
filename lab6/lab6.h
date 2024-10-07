#ifndef _LAB_6_
#define _LAB_6_

void read_lock(pthread_mutex_t* mutex);
void write_lock(pthread_mutex_t* mutex);
void write_unlock(pthread_mutex_t* mutex);
void read_unlock(pthread_mutex_t* mutex);
void init_my_rwlock();
void destroy_my_rwlock();


#endif