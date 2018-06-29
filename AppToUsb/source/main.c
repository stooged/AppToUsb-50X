#include "ps4.h"
#include "patch.h"

#define INI_FILE "AppToUsb.ini"

int nthread_run;
char notify_buf[1024];
char ini_file_path[256];
int  xfer_pct;
long xfer_cnt;
char *cfile;
int tmpcnt;
int isxfer;


void makeini()
{
    if (!file_exists(ini_file_path)) 
    {
    int ini = open(ini_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    char *buffer;
    buffer ="To check the usb root for the pkg file to save time copying from the internal ps4 drive then uncomment the line below.\r\nbut remember this will move the pkg from the root directory to the PS4 folder.\r\n//CHECK_USB\r\n\r\nTo rename previously linked pkg files to the new format uncomment the line below.\r\n//RENAME_APP\r\n\r\nTo disable the processing of icons/art and sound uncomment the line below.\r\n//DISABLE_META\r\n\r\nTo leave game updates on the internal drive uncomment the line below.\r\n//IGNORE_UPDATES\r\n\r\nTo move DLC to the usb hdd uncomment the line below.\r\n//MOVE_DLC\r\n\r\nTo use this list as a list of games you want to move not ignore then uncomment the line below.\r\n//MODE_MOVE\r\n\r\nExample ignore or move usage.\r\n\r\nCUSAXXXX1\r\nCUSAXXXX2\r\nCUSAXXXX3";
    write(ini, buffer, strlen(buffer));
    close(ini);
    }
}


char *getContentID(char* pkgFile)
{
        char buffer[37];
        char *retval = malloc(sizeof(char)*37);
        int pfile = open(pkgFile, O_RDONLY, 0);
        lseek (pfile, 64, SEEK_SET);
        read(pfile, buffer, sizeof(buffer));
        close(pfile);
        strcpy(retval, buffer);
        return retval;
}


char *getPkgName(char* sourcefile)
{
   char *retval = malloc(sizeof(char)*256);
   char *jfile = replace_str(sourcefile, ".pkg", ".json");
   if (file_exists(jfile)) 
   {
      int cfile = open(jfile, O_RDONLY, 0);
      char *idata = read_string(cfile);
      close(cfile);
      char *ret;
      ret = strstr(idata, "\"url\":\"");
      if (ret != NULL)
      {
         int bcnt = 0;
         char **buf = NULL;
         bcnt = split_string(ret, '/', &buf);
         split_string(buf[bcnt - 1], '"', &buf);
         if (strlen(buf[0]) > 0)
         {
            buf[0] = replace_str(buf[0], ".pkg", "");
            strcpy(retval, buf[0]);
            return retval;
         }
      }   
   }
        char *cid = getContentID(sourcefile);
        strcpy(retval, cid);
        free(cid);
        return retval;       
}


int isinlist(char *sourcefile)
{
        if (file_exists(ini_file_path)) 
        {
            int cfile = open(ini_file_path, O_RDONLY, 0);
            char *idata = read_string(cfile);
            close(cfile);
            if (strlen(idata) != 0)
            {
             char *tmpstr;
                if (strstr(sourcefile, "/user/app/") != NULL)
                {
                tmpstr = replace_str(sourcefile, "/user/app/", "");
                tmpstr = replace_str(tmpstr, "/app.pkg", "");
                }
                else if (strstr(sourcefile, "/user/patch/") != NULL)
                {
                tmpstr = replace_str(sourcefile, "/user/patch/", "");
                tmpstr = replace_str(tmpstr, "/patch.pkg", "");
                }
                else
                {
                tmpstr = replace_str(sourcefile, "/user/addcont/", "");
                char **buf = NULL;
                split_string(tmpstr,'/',&buf);
                tmpstr = buf[0];
                }
                if(strstr(idata, tmpstr) != NULL) 
                {
                   return 1;
                }
             return 0;
             }
        return 0;
        }
        else
        {
             return 0;
        }
}


int ismovemode()
{
        if (file_exists(ini_file_path)) 
        {
            int cfile = open(ini_file_path, O_RDONLY, 0);
            char *idata = read_string(cfile);
            close(cfile);
            if (strlen(idata) != 0)
            {
                if(strstr(idata, "//MODE_MOVE") != NULL) 
                {
                   return 0;
                }
                else if(strstr(idata, "MODE_MOVE") != NULL) 
                {
                   return 1;
                }
             return 0;
             }
        return 0;
        }
        else
        {
             return 0;
        }
}


int isusbcheck()
{
        if (file_exists(ini_file_path)) 
        {
            int cfile = open(ini_file_path, O_RDONLY, 0);
            char *idata = read_string(cfile);
            close(cfile);
            if (strlen(idata) != 0)
            {
                if(strstr(idata, "//CHECK_USB") != NULL) 
                {
                   return 0;
                }
                else if(strstr(idata, "CHECK_USB") != NULL) 
                {
                   return 1;
                }
             return 0;
             }
        return 0;
        }
        else
        {
             return 0;
        }
}


int isignupdates()
{
        if (file_exists(ini_file_path)) 
        {
            int cfile = open(ini_file_path, O_RDONLY, 0);
            char *idata = read_string(cfile);
            close(cfile);
            if (strlen(idata) != 0)
            {
                if(strstr(idata, "//IGNORE_UPDATES") != NULL) 
                {
                   return 0;
                }
                else if(strstr(idata, "IGNORE_UPDATES") != NULL) 
                {
                   return 1;
                }
             return 0;
             }
        return 0;
        }
        else
        {
             return 0;
        }
}


int isrelink()
{
        if (file_exists(ini_file_path)) 
        {
            int cfile = open(ini_file_path, O_RDONLY, 0);
            char *idata = read_string(cfile);
            close(cfile);
            if (strlen(idata) != 0)
            {
                if(strstr(idata, "//RENAME_APP") != NULL) 
                {
                   return 0;
                }
                else if(strstr(idata, "RENAME_APP") != NULL) 
                {
                   return 1;
                }
             return 0;
             }
        return 0;
        }
        else
        {
             return 0;
        }
}


int isnometa()
{
        if (file_exists(ini_file_path)) 
        {
            int cfile = open(ini_file_path, O_RDONLY, 0);
            char *idata = read_string(cfile);
            close(cfile);
            if (strlen(idata) != 0)
            {
                if(strstr(idata, "//DISABLE_META") != NULL) 
                {
                   return 0;
                }
                else if(strstr(idata, "DISABLE_META") != NULL) 
                {
                   return 1;
                }
             return 0;
             }
        return 0;
        }
        else
        {
             return 0;
        }
}


int isdlc()
{
        if (file_exists(ini_file_path)) 
        {
            int cfile = open(ini_file_path, O_RDONLY, 0);
            char *idata = read_string(cfile);
            close(cfile);
            if (strlen(idata) != 0)
            {
                if(strstr(idata, "//MOVE_DLC") != NULL) 
                {
                   return 0;
                }
                else if(strstr(idata, "MOVE_DLC") != NULL) 
                {
                   return 1;
                }
             return 0;
             }
        return 0;
        }
        else
        {
             return 0;
        }
}


void resetflags()
{
    if (file_exists(ini_file_path)) 
    {
       int cfile = open(ini_file_path, O_RDONLY, 0);
       char *idata = read_string(cfile);
       close(cfile);
       if (strlen(idata) != 0)
       {
          if(strstr(idata, "//RENAME_APP") == NULL) 
          {
             idata = replace_str(idata, "RENAME_APP", "//RENAME_APP");
          }
          int dfile = open(ini_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
          write(dfile, idata, strlen(idata));
          close(dfile);
       }
    }
}


void copyFile(char *sourcefile, char* destfile)
{
    int src = open(sourcefile, O_RDONLY, 0);
    if (src != -1)
    {
        int out = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (out != -1)
        {
            cfile = sourcefile;
            isxfer = 1;
            size_t bytes, bytes_size, bytes_copied = 0;
            char *buffer = malloc(65536);
            if (buffer != NULL)
            {
                lseek(src, 0L, SEEK_END);
                bytes_size = lseek(src, 0, SEEK_CUR);
                lseek(src, 0L, SEEK_SET);
                while (0 < (bytes = read(src, buffer, 65536))) {
                    write(out, buffer, bytes);
                    bytes_copied += bytes;
                    if (bytes_copied > bytes_size) bytes_copied = bytes_size;
                   xfer_pct = bytes_copied * 100 / bytes_size;
                   xfer_cnt += bytes;
                }
                free(buffer);
            }
            close(out);
            isxfer = 0;
            xfer_pct = 0;
            xfer_cnt = 0;
            unlink(sourcefile);
            symlink(destfile, sourcefile);
        }
        else {
        }
        close(src);
    }
    else {
    }
}


void copypkg(char *sourcepath, char* destpath)
{       
    if (!symlink_exists(sourcepath))
    {
        if (isfpkg(sourcepath) == 0) 
        {
            char cmsg[1024];
            char dstfile[256];
            char *ndestpath;
            char *pknm = getPkgName(sourcepath);
            sprintf(dstfile, "%s.pkg", pknm);
            free(pknm);
            if(strstr(sourcepath, "app.pkg") != NULL)
            {
                ndestpath = replace_str(destpath, "app.pkg", dstfile);
            }
            else if(strstr(sourcepath, "patch.pkg") != NULL)
            {
                ndestpath = replace_str(destpath, "patch.pkg", dstfile);
            }
            else
            {
                ndestpath = replace_str(destpath, "ac.pkg", dstfile);
            }
            if (file_exists(destpath)) 
            {
               rename(destpath, ndestpath);
            }
            if (!file_exists(ndestpath)) 
            {
                sprintf(cmsg, "%s\n%s", "Processing:" , sourcepath);
                systemMessage(cmsg);
                copyFile(sourcepath, ndestpath);
            }
            else
            {
                if (!file_compare(sourcepath, ndestpath))
                {
                    sprintf(cmsg, "%s\n%s\nOverwriting as pkg files are mismatched", "Found pkg at " , ndestpath);
                    systemMessage(cmsg);
                    copyFile(sourcepath, ndestpath);
                } 
                else
                {  
                    sprintf(cmsg, "%s\n%s\nSkipping copy and linking existing pkg", "Found pkg at " , ndestpath);
                    systemMessage(cmsg);
                    sceKernelSleep(5);
                    unlink(sourcepath);
                    symlink(ndestpath, sourcepath);
                }
            }
        }
    }
}


void checkusbpkg(char *sourcedir, char* destdir) {
   if (isusbcheck())
   {
     if (!symlink_exists(sourcedir))
     {
        if (isfpkg(sourcedir) == 0) 
        {
            char dstfile[256];
            char *pknm = getPkgName(sourcedir);
            sprintf(dstfile, "%s.pkg", pknm);
            free(pknm);
            if(strstr(sourcedir, "app.pkg") != NULL)
            {
                destdir = replace_str(destdir, "app.pkg", dstfile);
            }
            else if(strstr(sourcedir, "patch.pkg") != NULL)
            {
                destdir = replace_str(destdir, "patch.pkg", dstfile);
            }
            else
            {
                destdir = replace_str(destdir, "ac.pkg", dstfile);
            }
            if (!file_exists(destdir)) 
            {
            DIR *dir;
            struct dirent *dp;
            struct stat info;
            char upkg_path[1024];
            dir = opendir("/mnt/usb0/");
            if (dir) {
            while ((dp = readdir(dir)) != NULL)
            {
            if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") || !strcmp(dp->d_name, "$RECYCLE.BIN") || !strcmp(dp->d_name, "System Volume Information"))
            {}
            else
            {
            sprintf(upkg_path, "%s/%s", "/mnt/usb0", dp->d_name);
            if (!stat(upkg_path, &info))
            {
                if (S_ISREG(info.st_mode))
                {
		if (file_compare(sourcedir, upkg_path))
		{
		    rename(upkg_path, destdir);
		    break;
	        }
             }
          }
        }
      }
      closedir(dir);
      }
     }
   }
 }
}
}



void relink(char *sourcepath, char* destpath)
{
   if (isrelink())
   {
      if (symlink_exists(sourcepath))
      {
         char dstfile[256];
         char cidfile[50];
         char *ndestpath;
         if (file_exists(destpath)) 
         {
            char *pknm = getPkgName(sourcepath);
            sprintf(dstfile, "%s.pkg", pknm);
            free(pknm);
            if(strstr(destpath, "app.pkg") != NULL)
            {
                ndestpath = replace_str(destpath, "app.pkg", dstfile);
            }
            else
            {
                ndestpath = replace_str(destpath, "patch.pkg", dstfile);
            }
            rename(destpath, ndestpath);
            unlink(sourcepath);
            symlink(ndestpath, sourcepath);
         }

         else
         {
            char *cdestpath;
            char *cid = getContentID(sourcepath);
            sprintf(cidfile, "%s.pkg", cid);
            free(cid);
            if(strstr(sourcepath, "app.pkg") != NULL)
            {
                cdestpath = replace_str(destpath, "app.pkg", cidfile);
            }
            else
            {
                cdestpath = replace_str(destpath, "patch.pkg", cidfile);
            }
            if (file_exists(cdestpath)) 
            {
                char *pknm = getPkgName(sourcepath);
                sprintf(dstfile, "%s.pkg", pknm);
                free(pknm);
                ndestpath = replace_str(cdestpath, cidfile, dstfile);
                rename(cdestpath, ndestpath);
                unlink(sourcepath);
                symlink(ndestpath, sourcepath);
            }
         }
      }
   }
}




void copyMeta(char *sourcedir, char* destdir, int tousb)
{
    DIR *dir;
    struct dirent *dp;
    struct stat info;
    char src_path[1024], dst_path[1024];
    dir = opendir(sourcedir);
    if (!dir)
        return;
        mkdir(destdir, 0777);
    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") || !strcmp(dp->d_name, "$RECYCLE.BIN") || !strcmp(dp->d_name, "System Volume Information"))
        {}
        else
        {
            sprintf(src_path, "%s/%s", sourcedir, dp->d_name);
            sprintf(dst_path, "%s/%s", destdir  , dp->d_name);

            if (!stat(src_path, &info))
            {
                if (S_ISDIR(info.st_mode))
                {
                  copyMeta(src_path, dst_path, tousb);
                }
                else
                if (S_ISREG(info.st_mode))
                {
                   if(strstr(src_path, ".png") != NULL || strstr(src_path, ".at9") != NULL)
                   { 
                      if (tousb == 1)
                      {
                         if (!file_exists(dst_path)) 
                         {
                            copy_File(src_path, dst_path);
                         }
                      }
                      else
                      {
                         if (file_exists(dst_path)) 
                         {
                         if (!file_compare(src_path, dst_path))
                            {
                               copy_File(src_path, dst_path);
                            }
                         }
                      }
                   }
                }
            }
        }
    }
    closedir(dir);
}



void makePkgInfo(char *pkgFile, char *destpath)
{
    if(strstr(pkgFile, "app.pkg") != NULL && !isnometa())
    {
        char *titleid;
        char srcfile[256];
        char dstfile[256];
        destpath = replace_str(destpath, "/app.pkg", "");
        titleid = replace_str(pkgFile, "/user/app/", "");
        titleid = replace_str(titleid, "/app.pkg", "");
        sprintf(srcfile, "/user/appmeta/%s/pronunciation.xml", titleid);
        if (file_exists(srcfile))
        {
           char *cid = getContentID(pkgFile);
           sprintf(dstfile, "%s/%s.txt", destpath , cid);
           free(cid);
           if (!file_exists(dstfile))
           {
              copy_File(srcfile, dstfile);
           }
        }
         sprintf(srcfile, "/user/appmeta/%s", titleid);
         copyMeta(srcfile, destpath, 1);
    }
}



void copyDir(char *sourcedir, char* destdir)
{
    DIR *dir;
    struct dirent *dp;
    struct stat info;
    char src_path[1024], dst_path[1024];
    dir = opendir(sourcedir);
    if (!dir)
        return;
        mkdir(destdir, 0777);
    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
        {}
        else
        {
            sprintf(src_path, "%s/%s", sourcedir, dp->d_name);
            sprintf(dst_path, "%s/%s", destdir  , dp->d_name);

            if (!stat(src_path, &info))
            {
                if (S_ISDIR(info.st_mode))
                {
                  copyDir(src_path, dst_path);
                }
                else
                if (S_ISREG(info.st_mode))
                {
                  if(strstr(src_path, "app.pkg") != NULL || strstr(src_path, "patch.pkg") != NULL || strstr(src_path, "ac.pkg") != NULL) 
                  {
                   if (ismovemode() )
                   {
                   if (isinlist(src_path) )
                   {
                     relink(src_path, dst_path);
                     makePkgInfo(src_path, dst_path);
                     checkusbpkg(src_path, dst_path);
                     copypkg(src_path, dst_path);
                   }
                 }
                 else
                 {
                   if (!isinlist(src_path) )
                   {
                     relink(src_path, dst_path);
                     makePkgInfo(src_path, dst_path);
                     checkusbpkg(src_path, dst_path);
                     copypkg(src_path, dst_path);
                   }
                 }
               }
             }
          }
       }
    }
    closedir(dir);
}


void *nthread_func(void *arg)
{
        time_t t1, t2;
        t1 = 0;
	while (nthread_run)
	{
		if (isxfer)
		{
			t2 = time(NULL);
			if ((t2 - t1) >= 20)
			{
				t1 = t2;
				if (tmpcnt >= 1048576)
				{
				   sprintf(notify_buf, "Copying: %s\n\n%u%% completed\nSpeed: %u MB/s", cfile , xfer_pct, tmpcnt / 1048576);
				}
				else if (tmpcnt >= 1024)
				{
				   sprintf(notify_buf, "Copying: %s\n\n%u%% completed\nSpeed: %u KB/s", cfile , xfer_pct, tmpcnt / 1024);
				}
				else
				{
				   sprintf(notify_buf, "Copying: %s\n\n%u%% completed\nSpeed: %u B/s", cfile , xfer_pct, tmpcnt);
				}
				
				systemMessage(notify_buf);
			}
		}
		else t1 = 0;
		sceKernelSleep(1);
	}
	return NULL;
}


void *sthread_func(void *arg)
{
	while (nthread_run)
	{
           if (isxfer)
           {
              tmpcnt = xfer_cnt;
              xfer_cnt = 0;
           }
          sceKernelSleep(1);
	}
	return NULL;
}



int _main(struct thread *td) {
    initKernel();
    initLibc();
    initPthread();
    DIR *dir;


    dir = opendir("/user/app");
    if (!dir)
    {
       syscall(11,patcher,td);

    }
    else
    {
       closedir(dir);
    }

    initSysUtil();
        xfer_cnt = 0;
        isxfer = 0;
	nthread_run = 1;
	ScePthread nthread;
	scePthreadCreate(&nthread, NULL, nthread_func, NULL, "nthread");
	ScePthread sthread;
	scePthreadCreate(&sthread, NULL, sthread_func, NULL, "sthread");
        systemMessage("Warning this payload will modify the filesystem on your PS4\n\nUnplug your usb drive to cancel this");
        sceKernelSleep(10);
        systemMessage("Last warning\n\nUnplug your usb drive to cancel this");
        sceKernelSleep(10);
        int usbdir = open("/mnt/usb0/.dirtest", O_WRONLY | O_CREAT | O_TRUNC, 0777);
         if (usbdir == -1)
            {
                  systemMessage("No usb mount found.\nYou must use a eXfat formatted usb hdd\nThe USB drive must be plugged into USB0");
                  nthread_run = 0;
                  return 0;
            }
            else
            {
                        close(usbdir);
                        unlink("/mnt/usb0/.dirtest");
                        mkdir("/mnt/usb0/PS4/", 0777);
                        sprintf(ini_file_path, "/mnt/usb0/%s", INI_FILE);
                        if (!file_exists(ini_file_path))
                        {
                        sprintf(ini_file_path, "/mnt/usb0/PS4/%s", INI_FILE);
                        makeini();
                        }
                        systemMessage("Copying Apps to USB0\n\nThis will take a while if you have lots of games installed");
                        copyDir("/user/app","/mnt/usb0/PS4");
                        if (!isignupdates())
                        {
                           mkdir("/mnt/usb0/PS4/updates/", 0777);
                           systemMessage("Copying updates to USB0");
                           copyDir("/user/patch","/mnt/usb0/PS4/updates");
                        }
                        if (isdlc())
                        {
                           mkdir("/mnt/usb0/PS4/dlc/", 0777);
                           systemMessage("Copying dlc to USB0");
                           copyDir("/user/addcont","/mnt/usb0/PS4/dlc");
                        }
                        if (!isnometa())
                        {
                           systemMessage("Processing appmeta");
                           copyMeta("/mnt/usb0/PS4","/user/appmeta", 0);
                        }
                        if (isrelink())
                        {
                           resetflags();
                        }
                        systemMessage("Complete.");
            }
    nthread_run = 0;
    return 0;
}


