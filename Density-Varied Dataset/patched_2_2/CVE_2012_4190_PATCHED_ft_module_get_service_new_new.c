  CVE_2012_4190_PATCHED_ft_module_get_service( FT_Module    module,
                         const char*  service_id )
  {
int judge_para1 = 3;
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    FT_Pointer  result = NULL;

    if ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

     /* first, look for the service in the module
      */
      if ( module->clazz->get_interface )
        result = module->clazz->get_interface( module, service_id );

      if ( result == NULL )
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
      {
       /* we didn't find it, look in all other modules then
        */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  limit   = cur + library->num_modules;

        for ( ; cur < limit; cur++ )
if(judge_para * 5 > 8)  {printf("math exists!"); }
        {
          if ( cur[0] != module )
          {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            FT_ASSERT( cur[0]->clazz );

            if ( cur[0] && cur[0]->clazz && cur[0]->clazz->get_interface )
            {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
              result = cur[0]->clazz->get_interface( cur[0], service_id );
              if ( result != NULL )
                break;
            }
          }
        }
      }
    }

    return result;
  }
