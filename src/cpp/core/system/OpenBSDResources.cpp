/*
 * OpenBSDResources.cpp
 */

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/resource.h>
#include <unistd.h>

#include <shared_core/Error.hpp>
#include <core/system/Resources.hpp>

namespace rstudio {
namespace core {
namespace system {

Error getTotalMemory(long* pTotalKb, MemoryProvider* pProvider)
{
   int mib[2] = { CTL_HW, HW_PHYSMEM64 };
   uint64_t mem = 0;
   size_t len = sizeof(mem);

   if (::sysctl(mib, 2, &mem, &len, nullptr, 0) == -1)
      return systemError(errno,
                         "Failed to get total physical memory from sysctl",
                         ERROR_LOCATION);

   *pTotalKb = static_cast<long>(mem / 1024);
   *pProvider = MemoryProviderUnknown;
   return Success();
}

Error getTotalMemoryUsed(long* pUsedKb, MemoryProvider* pProvider)
{
   int mib[2] = { CTL_VM, VM_UVMEXP };
   struct uvmexp uvm;
   size_t len = sizeof(uvm);

   if (::sysctl(mib, 2, &uvm, &len, nullptr, 0) == -1)
      return systemError(errno,
                         "Failed to get VM statistics from sysctl",
                         ERROR_LOCATION);

   long usedPages =
      static_cast<long>(uvm.npages) -
      static_cast<long>(uvm.free);

   *pUsedKb =
      usedPages * static_cast<long>(uvm.pagesize) / 1024;

   *pProvider = MemoryProviderUnknown;
   return Success();
}

Error getProcessMemoryUsed(long* pUsedKb, MemoryProvider* pProvider)
{
   struct kinfo_proc info;
   size_t len = sizeof(info);

   int mib[4] =
   {
      CTL_KERN,
      KERN_PROC,
      KERN_PROC_PID,
      static_cast<int>(::getpid())
   };

   if (::sysctl(mib, 4, &info, &len, nullptr, 0) == -1)
      return systemError(errno,
                         "Failed to get process memory usage from sysctl",
                         ERROR_LOCATION);

   *pUsedKb =
      static_cast<long>(info.p_vm_rssize) *
      static_cast<long>(::getpagesize()) / 1024;

   *pProvider = MemoryProviderUnknown;
   return Success();
}

Error getProcessMemoryLimit(long* pLimitKb, MemoryProvider* pProvider)
{
   struct rlimit limit;

   if (::getrlimit(RLIMIT_RSS, &limit) == -1)
      return systemError(errno,
                         "Failed to get process memory limit",
                         ERROR_LOCATION);

   if (limit.rlim_cur == RLIM_INFINITY)
      *pLimitKb = 0;
   else
      *pLimitKb = static_cast<long>(limit.rlim_cur / 1024);

   *pProvider = MemoryProviderUnknown;
   return Success();
}

Error getProcessCpuLimit(double* pNumCpus, MemoryProvider* pProvider)
{
   *pNumCpus = 0.0;
   *pProvider = MemoryProviderUnknown;
   return Success();
}

long getProcessSize()
{
   long processMemoryUsedKb = 0;
   MemoryProvider provider = MemoryProviderUnknown;

   Error error =
      getProcessMemoryUsed(&processMemoryUsedKb, &provider);

   if (error)
      return 0;

   return processMemoryUsedKb;
}

} // namespace system
} // namespace core
} // namespace rstudio
