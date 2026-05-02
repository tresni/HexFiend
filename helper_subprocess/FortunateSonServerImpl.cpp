#define INDENT_HIDDEN_FROM_XCODE {
#define UNINDENT_HIDDEN_FROM_XCODE }

extern "C" INDENT_HIDDEN_FROM_XCODE

#include "FortunateSonServer.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "fileport.h"
#include <Security/Authorization.h>

static AuthorizationRef authRef;

kern_return_t _FortunateSonOpenFile(mach_port_t, FilePath path, int writable, fileport_t *fd_port, int *err) {
	char *right_name;
	asprintf(&right_name, "sys.openfile.%s.%s",
			 writable ? "readwritecreate" : "readonly",
			 path);

	AuthorizationItem right = {
		.name = right_name
	};
	
	AuthorizationRights rights = {
		.count = 1,
		.items = &right
	};

	OSStatus status = AuthorizationCopyRights(authRef, &rights, NULL, kAuthorizationFlagExtendRights | kAuthorizationFlagInteractionAllowed, NULL);
	free (right_name);

	if (status == errAuthorizationCanceled) {
		*fd_port = MACH_PORT_NULL;
		*err = ECANCELED;
		return KERN_SUCCESS;
	}
	if (status) {
		*fd_port = MACH_PORT_NULL;
		*err = EACCES;
		return KERN_SUCCESS;
	}

	int fd = open(path, writable ? O_RDWR | O_CREAT : O_RDONLY, S_IRUSR|S_IWUSR);

	if (fd < 0) {
		*fd_port = MACH_PORT_NULL;
		*err = errno;
		return KERN_SUCCESS;
	}
	
	if (fileport_makeport(fd, fd_port)) {
		*fd_port = MACH_PORT_NULL;
		*err = errno;
		perror("fileport_makeport failed");
		close(fd);
		return KERN_SUCCESS;
	}

	if (close(fd))
		perror("close failed");
	*err = 0;

	return KERN_SUCCESS;
}

kern_return_t _FortunateSonSetAuthorization(mach_port_t, AuthorizationExternalForm authExt) {
	AuthorizationCreateFromExternalForm(&authExt, &authRef);
	return KERN_SUCCESS;
}

//extern C
UNINDENT_HIDDEN_FROM_XCODE
