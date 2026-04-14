// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_FS_VFS_H
#define POVOS_FS_VFS_H

//
// Virtual File System
// ===================
//
// Common interface for all filesystems.
//

#include <libk/stddef.h>

// forward declarations
typedef struct vgfmount vfsmount_t;
typedef struct inode inode_t;
typedef struct vfs vfs_t;

typedef u64_t inode_no_t;

//
// File
// ----
//
// The file object represents an open resource that is being used. It
// is owned by a mounted filesystem.
//
typedef struct file {
  inode_t *inode;
  u8_t     mode;
  u64_t    offset;

  vfsmount_t *owner;
} file_t;

typedef struct file_ops {
  #define FILE_OPS_FLAG_CREATE   1
  #define FILE_OPS_FLAG_APPEND   2
  #define FILE_OPS_MODE_READ     0
  #define FILE_OPS_MODE_WRITE    1
  file_t* (*open)   (inode_t *inode, u8_t flags, u8_t mode);
  int     (*close)  (file_t *file);
  size_t  (*read)   (file_t *file, u8_t *dest, size_t count);
  size_t  (*write)  (file_t *file, u8_t *src, size_t count);
  #define FILE_OPS_SEEK_SET   0
  #define FILE_OPS_SEEK_CUR   1
  #define FILE_OPS_SEEK_END   2
  u64_t   (*lseek)  (file_t *file, size_t offset, u8_t whence);
} file_ops_t;

//
// Inode
// -----
//
// This is the lower level abstraction the VFS provides. It represents
// an object in its filesystem. What this means fully depends on the
// implementation of the filesystem, for example it may represent a
// memory rangeon the disk and its metadata (drive, sector...).
//
// The name is quite confusing, but I decided to keep the standard
// UNIX naming since we are used to it.
//
struct inode {
  // Unique identifier
  inode_no_t  inode_no;
  inode_t    *next;

  // Operations to interact with the inode, they are here so that
  // different types of inodes may support different ops
  file_ops_t  file_ops;
};

//
// Directory Entry
// ---------------
//
// The leaf of a directory.
//
typedef struct dentry dentry_t;

struct dentry {
  char         *name;
  inode_t      *inode;
  // Mount for another filesystem, may be null
  vfsmount_t   *mount;

  dentry_t  *parent;
  // Other entries in the same directory
  dentry_t  *next;
};

//
// Directory
// ---------
//
// Directories form tree where leafs are dentries. A directory is
// owned by a mounted filesystem.
//
typedef struct dir dir_t;

struct dir {
  // Dentry representing this directory
  dentry_t *self;
  dentry_t *entries;

  vfsmount_t *owner;
};

typedef struct dir_ops {
  dir_t* (*mkdir) (dir_t *father);
  int    (*rmdir) (dir_t *dir);
} dir_ops_t;

//
// The Filesystem object
// ---------------------
//
// This is the higher level structure of the filesystem. It implements
// several operations for the filesystem itself and its files.
//

typedef struct vfs_ops {
  vfsmount_t *(*mount)  (dir_t *dir, u64_t flags);
  int        *(*umount) (vfsmount_t *mount);
  // Returns an inode by walking the directory
  inode_t    *(*lookup) (dir_t *start, const char *path);
} vfs_ops_t;

struct vfs {
  // Name used for printing
  const char*      name;
  // Identifier of the implementation
  u64_t  type;

  // API
  dir_ops_t   dir_ops;
  vfs_ops_t   vfs_ops;
};

//
// Superblock
// ----------
//
// A superblock is an instance of a filesystem.
//
typedef struct vfs_superblock {
  vfs_t *fs;
  // An unique identifier of this instance
  u64_t id;
  // Root directory of the filesystem
  dir_t   *root;
} vfs_superblock_t;

//
// Mountpoint
// ----------
//
// Represents an active filesystem.
//
struct vfsmount {
  // The mounted filesystem
  vfs_t  *fs;
  u64_t   flags;

  // Filesystem insance
  vfs_superblock_t  *sb;

  // Next mount in the list
  vfsmount_t *next;
};

#endif // POVOS_FS_VFS_H
