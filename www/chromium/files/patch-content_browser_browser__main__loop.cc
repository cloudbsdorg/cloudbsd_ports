--- content/browser/browser_main_loop.cc.orig	2022-03-25 21:59:56 UTC
+++ content/browser/browser_main_loop.cc
@@ -240,6 +240,12 @@
 #include "mojo/public/cpp/bindings/lib/test_random_mojo_delays.h"
 #endif
 
+#if BUILDFLAG(IS_BSD)
+#include "content/browser/sandbox_host_linux.h"
+#include "content/public/common/zygote/sandbox_support_linux.h"
+#include "sandbox/policy/sandbox.h"
+#endif
+
 // One of the linux specific headers defines this as a macro.
 #ifdef DestroyAll
 #undef DestroyAll
@@ -547,6 +553,12 @@ int BrowserMainLoop::EarlyInitialization() {
   // by now since a thread to start the ServiceManager has been created
   // before the browser main loop starts.
   DCHECK(SandboxHostLinux::GetInstance()->IsInitialized());
+#elif BUILDFLAG(IS_BSD)
+  base::FileHandleMappingVector additional_remapped_fds;
+  base::LaunchOptions options;
+  SandboxHostLinux::GetInstance()->Init();
+  const int sfd = SandboxHostLinux::GetInstance()->GetChildSocket();
+  options.fds_to_remap.push_back(std::make_pair(sfd, GetSandboxFD()));
 #endif
 
   // GLib's spawning of new processes is buggy, so it's important that at this
@@ -574,7 +586,7 @@ int BrowserMainLoop::EarlyInitialization() {
 
   // Up the priority of the UI thread unless it was already high (since Mac
   // and recent versions of Android (O+) do this automatically).
-#if !BUILDFLAG(IS_MAC)
+#if !BUILDFLAG(IS_MAC) && !BUILDFLAG(IS_BSD)
   if (base::FeatureList::IsEnabled(
           features::kBrowserUseDisplayThreadPriority) &&
       base::PlatformThread::GetCurrentThreadPriority() <
@@ -585,7 +597,7 @@ int BrowserMainLoop::EarlyInitialization() {
 #endif  // !BUILDFLAG(IS_MAC)
 
 #if BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_CHROMEOS) || \
-    BUILDFLAG(IS_ANDROID)
+    BUILDFLAG(IS_ANDROID) || BUILDFLAG(IS_BSD)
   // We use quite a few file descriptors for our IPC as well as disk the disk
   // cache,and the default limit on the Mac is low (256), so bump it up.
 
