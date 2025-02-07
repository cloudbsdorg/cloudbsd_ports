--- gpu/command_buffer/service/webgpu_decoder_impl.cc.orig	2022-03-25 21:59:56 UTC
+++ gpu/command_buffer/service/webgpu_decoder_impl.cc
@@ -1498,7 +1498,7 @@ error::Error WebGPUDecoderImpl::HandleRequestAdapter(
   }
 
   if (gr_context_type_ != GrContextType::kVulkan) {
-#if BUILDFLAG(IS_LINUX)
+#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_BSD)
     SendAdapterProperties(request_adapter_serial, -1, nullptr,
                           "WebGPU on Linux requires command-line flag "
                           "--enable-features=Vulkan,UseSkiaRenderer");
