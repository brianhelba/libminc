#include <stdio.h>
#include <string.h>
#include "minc2.h"

#define TESTRPT(msg, val) (error_cnt++, fprintf(stderr, \
"Error reported on line #%d, %s: %d\n", \
__LINE__, msg, val))

#define TESTARRAYSIZE 11

static int error_cnt = 0;

int main(void)
{
  mihandle_t hvol;
  mihandle_t hvol1;
  int r;
  mitype_t data_type;
  size_t length;
  static double tstarr[TESTARRAYSIZE] = {
    1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10, 11.11
  };
  double dblarr[TESTARRAYSIZE];
  float fltarr[TESTARRAYSIZE];
  int intarr[TESTARRAYSIZE];
  char valstr[128]="";
  float val1=12.5;
  float val2=34.5;
  milisthandle_t hlist, h1list;
  char pathbuf[256]="";
  char namebuf[256]="";
  char pathbuf1[1024]="";
  int count=0;
  
  r = micreate_volume("tst-grpa.mnc", 0, NULL, MI_TYPE_UINT,
                      MI_CLASS_REAL, NULL, &hvol);
  if (r < 0) {
    TESTRPT("Unable to create test file", r);
    return (-1);
  }
  r = micreate_volume("tst-grpb.mnc", 0, NULL, MI_TYPE_UINT,
                      MI_CLASS_REAL, NULL, &hvol1);
  if (r < 0) {
    TESTRPT("Unable to create test file", r);
    return (-1);
  }
  
  r = micreate_group(hvol, "/", "test1");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  
  r = micreate_group(hvol, "/", "test2");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/", "test3");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/", "test4");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/test2", "stuff2");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/test1", "stuff");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/test1", "otherstuff");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/test1", "theotherstuff");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/test1/theotherstuff", "thisstuff");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/test1/stuff", "hello");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  r = micreate_group(hvol, "/test1/stuff", "helloleila");
  if (r < 0) {
    TESTRPT("micreate_group failed", r);
  }
  
  r = miset_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff/hello",
                        "animal", 8, "fruitbat");
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }
  
  r = miset_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff",
                        "objtype", 10, "automobile");
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }
  
  r = miset_attr_values(hvol, MI_TYPE_STRING, "/test3",
                        "objtype", 10, "automobile");
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }
  
  r = miset_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff",
                        "objname", 10, "automobile");
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }
  r = miset_attr_values(hvol, MI_TYPE_DOUBLE, "/test2",
                        "maxvals", TESTARRAYSIZE, tstarr);
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }
  
  r = miget_attr_type(hvol, "/test1/stuff/hello", "animal",
                      &data_type);
  if (r < 0) {
    TESTRPT("miget_attr_type failed", r);
  }
  if (data_type != MI_TYPE_STRING) {
    TESTRPT("miget_attr_type failed", data_type);
  }
  
  r = miget_attr_length(hvol, "/test1/stuff/hello", "animal",
                        &length);
  if (r < 0) {
    TESTRPT("miget_attr_length failed", r);
  }
  
  if (length != 8) {
    TESTRPT("miget_attr_length failed", (int)length);
  }
  
  r = midelete_group(hvol, "/test1/stuff", "goodbye");
  if (r >= 0) {
    TESTRPT("midelete_group failed", r);
  }
  
  r = midelete_group(hvol, "/test1/stuff", "hello");
  /* This should succeed.
   */
  if (r < 0) {
    TESTRPT("midelete_group failed", r);
  }
  
  r = miget_attr_length(hvol, "/test1/stuff/hello", "animal",
                        &length);
  /* This should fail since we deleted the group.
   */
  if (r >= 0) {
    TESTRPT("miget_attr_length not failed", r);
  }
  
  r = miget_attr_values(hvol, MI_TYPE_DOUBLE, "/test2", "maxvals",
                        TESTARRAYSIZE, dblarr);
  if (r < 0) {
    TESTRPT("miget_attr_values failed", r);
  }
  
  for (r = 0; r < TESTARRAYSIZE; r++) {
    if (dblarr[r] != tstarr[r]) {
	    TESTRPT("miget_attr_values mismatch", r);
    }
  }
  
  /* Get the values again in float rather than double format.
   */
  r = miget_attr_values(hvol, MI_TYPE_FLOAT, "/test2", "maxvals",
                        TESTARRAYSIZE, fltarr);
  if (r < 0) {
    TESTRPT("miget_attr_values failed", r);
  }
  
  for (r = 0; r < TESTARRAYSIZE; r++) {
    if (fltarr[r] != (float) tstarr[r]) {
	    TESTRPT("miget_attr_values mismatch", r);
	    fprintf(stderr, "fltarr[%d] = %f, tstarr[%d] = %f\n",
              r, fltarr[r], r, tstarr[r]);
    }
  }
  
  /* Get the values again in int rather than double format.
   */
  r = miget_attr_values(hvol, MI_TYPE_INT, "/test2", "maxvals",
                        TESTARRAYSIZE, intarr);
  if (r < 0) {
    TESTRPT("miget_attr_values failed", r);
  }
  
  for (r = 0; r < TESTARRAYSIZE; r++) {
    if (intarr[r] != (int) tstarr[r]) {
	    TESTRPT("miget_attr_values mismatch", r);
	    fprintf(stderr, "intarr[%d] = %d, tstarr[%d] = %d\n",
              r, intarr[r], r, (int) tstarr[r]);
    }
  }
  
  r = miget_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff",
                        "objtype", 128, valstr);
  if (r < 0) {
    TESTRPT("miget_attr_values failed", r);
  }
  
  if (strcmp(valstr, "automobile") != 0) {
    TESTRPT("miget_attr_values failed", 0);
    fprintf(stderr,"Expected :\"%s\" read \"%s\"\n","automobile",valstr);
  }

  /* Get the values again but this time with only enough space
     for the result with null termination.
   */
  memset(valstr, 0x55, sizeof(valstr));
  r = miget_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff",
                        "objtype", 11, valstr);
  if (r < 0) {
    TESTRPT("miget_attr_values failed", r);
  }
  if (strcmp(valstr, "automobile") != 0) {
    TESTRPT("miget_attr_values failed", 0);
  }

  /* Get the values again but this time with only enough space
     for the result and without null termination.
   */
  memset(valstr, 0x55, sizeof(valstr));
  r = miget_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff",
                        "objtype", 10, valstr);
  if (r < 0) {
    TESTRPT("miget_attr_values failed", r);
  }
  if (valstr[9] != 'e') {
    TESTRPT("miget_attr_values failed", 0);
  }
  if (valstr[10] != 0x55) {
    TESTRPT("miget_attr_values failed", 0);
  }

  r = miset_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff",
                        "objtype", 8, "bicycle");
  
  if (r < 0) {
    TESTRPT("miset_attr_values failed on rewrite", r);
  }
  
  r = miget_attr_values(hvol, MI_TYPE_STRING, "/test1/stuff",
                        "objtype", 128, valstr);
  if (r < 0) {
    TESTRPT("miget_attr_values failed", r);
  }
  
  if (strcmp(valstr, "bicycle") != 0) {
    TESTRPT("miget_attr_values failed", 0);
  }
  r = miset_attr_values(hvol, MI_TYPE_FLOAT, "/OPT",
                        "zoom",1, &val1);
  
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }
  
  r = miset_attr_values(hvol, MI_TYPE_FLOAT, "/OPT",
                        "binning",1, &val2);
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }
  
  r = miset_attr_values(hvol, MI_TYPE_FLOAT, "/OPT",
                        "gain",1, &val1);
  if (r < 0) {
    TESTRPT("miset_attr_values failed", r);
  }

  r = milist_start(hvol, "/", 0, &hlist);
  if (r == MI_NOERROR) {
    count++;
    while (milist_attr_next(hvol, hlist, pathbuf, sizeof(pathbuf),
                            namebuf, sizeof(namebuf)) == MI_NOERROR) {
      printf(" %s %s\n", pathbuf, namebuf);
    }
  } else {
    TESTRPT("milist_start failed", r);
  }
  milist_finish(hlist);
  
  printf("copy all attributes in the provided path in the new volume\n");
  if((r = micopy_attr(hvol,"/OPT",hvol1))<0) 
    TESTRPT("micopy_attr failed", r);
  printf("***************** \n");

  r = milist_start(hvol1, "/", 1, &h1list);
  if (r == MI_NOERROR) {
    while( milist_grp_next(h1list, pathbuf1, sizeof(pathbuf1)-1) == MI_NOERROR) {
      printf("%s \n", pathbuf1);
    }
  } else {
    TESTRPT("milist_start failed", r);  
  }
  
  r = milist_finish(h1list);
  if(r<0)
  {
    TESTRPT("milist_finish failed", r);  
  }

  r = miclose_volume(hvol1);
  if(r<0)
  {
    TESTRPT("miclose_volume failed", r);  
  }
  
  r = miclose_volume(hvol);
  if(r<0)
  {
    TESTRPT("miclose_volume failed", r);  
  }
  
  if (error_cnt != 0) {
    fprintf(stderr, "%d error%s reported\n",
            error_cnt, (error_cnt == 1) ? "" : "s");
  } else {
    fprintf(stderr, "No errors\n");
  }
  
  return (error_cnt);
}

