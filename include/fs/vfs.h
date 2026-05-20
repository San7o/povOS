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
struct vgfmount;
struct inode;
struct vfs;

typedef u64_t inode_no_t;

//
// File
// ----
//
// The file object represents an open resource that is being used. It
// is owned by a mounted filesystem.
//
struct file {
  struct inode *inode;
  u8_t  mode;
  u64_t offset;

  struct vfsmount *owner;
};

struct file_ops {
  #define FILE_OPS_FLAG_CREATE   1
  #define FILE_OPS_FLAG_APPEND   2
  #define FILE_OPS_MODE_READ     0
  #define FILE_OPS_MODE_WRITE    1
  struct file* (*open) (struct inode *inode, u8_t flags, u8_t mode);
  int     (*close)  (struct file *file);
  size_t  (*read)   (struct file *file, u8_t *dest, size_t count);
  size_t  (*write)  (struct file *file, u8_t *src, size_t count);
  #define FILE_OPS_SEEK_SET   0
  #define FILE_OPS_SEEK_CUR   1
  #define FILE_OPS_SEEK_END   2
  u64_t   (*lseek)  (struct file *file, size_t offset, u8_t whence);
};

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
  inode_no_t inode_no;
  struct inode *next;

  // Operations to interact with the inode, they are here so that
  // different types of inodes may support different ops
  struct file_ops file_ops;
};

//
// Directory Entry
// ---------------
//
// The leaf of a directory.
//
struct dentry {
  char *name;
  struct inode *inode;
  // Mount for another filesystem, may be null
  struct vfsmount *mount;

  struct dentry *parent;
  // Other entries in the same directory
  struct dentry  *next;
};

//
// Directory
// ---------
//
// Directories form tree where leafs are dentries. A directory is
// owned by a mounted filesystem.
//
struct dir {
  // Dentry representing this directory
  struct dentry *self;
  struct dentry *entries;

  struct vfsmount *owner;
};

struct dir_ops {
  struct dir* (*mkdir) (struct dir *father);
  int         (*rmdir) (struct dir *dir);
};

//
// The Filesystem object
// ---------------------
//
// This is the higher level structure of the filesystem. It implements
// several operations for the filesystem itself and its files.
//

struct vfs_ops {
  struct vfsmount *(*mount) (struct dir *dir, u64_t flags);
  int        *(*umount) (struct vfsmount *mount);
  // Returns an inode by walking the directory
  struct inode    *(*lookup) (struct dir *start, const char *path);
};

struct vfs {
  // Name used for printing
  const char*      name;
  // Identifier of the implementation
  u64_t  type;

  // API
  struct dir_ops   dir_ops;
  struct vfs_ops   vfs_ops;
};

//
// Superblock
// ----------
//
// A superblock is an instance of a filesystem.
//
struct vfs_superblock {
  struct  vfs *fs;
  // An unique identifier of this instance
  u64_t id;
  // Root directory of the filesystem
  struct dir  *root;
};

//
// Mountpoint
// ----------
//
// Represents an active filesystem.
//
struct vfsmount {
  // The mounted filesystem
  struct vfs_t  *fs;
  u64_t  flags;

  // Filesystem insance
  struct vfs_superblock  *sb;

  // Next mount in the list
  struct vfsmount  *next;
};

#endif // POVOS_FS_VFS_H
