//
//  HFPrivilegedHelperConnection.h
//  HexFiend_2
//
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <HexFiend/HFFrameworkPrefix.h>
#import <HexFiend/HFPrivilegedHelper.h>

NS_ASSUME_NONNULL_BEGIN

@interface HFPrivilegedHelperConnection : NSObject <HFPrivilegedHelper> {
    NSMachPort *childReceiveMachPort;
}

@property BOOL disabled; ///< When set, fail all requests as if the connection failed.

+ (instancetype)sharedConnection;
- (BOOL)launchAndConnect:(NSError **)error;
- (BOOL)connectIfNecessary;

- (BOOL)openFileAtPath:(const char *)path writable:(BOOL)writable fileDescriptor:(int *)outFD error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
