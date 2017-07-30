/**
*   Copyright(C) 2011-2016 Intel Corporation All Rights Reserved.
*
*   The source code, information  and  material ("Material") contained herein is
*   owned  by Intel Corporation or its suppliers or licensors, and title to such
*   Material remains  with Intel Corporation  or its suppliers or licensors. The
*   Material  contains proprietary information  of  Intel or  its  suppliers and
*   licensors. The  Material is protected by worldwide copyright laws and treaty
*   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
*   modified, published, uploaded, posted, transmitted, distributed or disclosed
*   in any way  without Intel's  prior  express written  permission. No  license
*   under  any patent, copyright  or  other intellectual property rights  in the
*   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
*   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
*   intellectual  property  rights must  be express  and  approved  by  Intel in
*   writing.
*
*   *Third Party trademarks are the property of their respective owners.
*
*   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
*   this  notice or  any other notice embedded  in Materials by Intel or Intel's
*   suppliers or licensors in any way.
*/
#include <stdio.h>

#include "../App.h"
#include "Enclave_u.h"
#include <thread>

/* ecall_libcxx_functions:
 *   Invokes standard C++11 functions.
 */

 //This function is part of mutex demo
void demo_counter_without_mutex()
{
	sgx_status_t ret = SGX_ERROR_UNEXPECTED;
	ret = ecall_mutex_demo_no_protection(global_eid);
	if (ret != SGX_SUCCESS)
		abort();
}

//This function is part of mutex demo
void demo_counter_mutex()
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = ecall_mutex_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();
}

//This function is used by processing thread of condition variable demo
void demo_cond_var_run()
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = ecall_condition_variable_run(global_eid);
    if (ret != SGX_SUCCESS)
        abort();
}

//This function is used by the loader thread of condition variable demo
void demo_cond_var_load()
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = ecall_condition_variable_load(global_eid);
    if (ret != SGX_SUCCESS)
        abort();
}

// Examples for C++11 library and compiler features
void ecall_libcxx_functions(void)
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    
    // Example for lambda function feature:
    ret = ecall_lambdas_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for auto feature:
    ret = ecall_auto_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for decltype:
    ret = ecall_decltype_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for strongly_typed_enum:
    ret = ecall_strongly_typed_enum_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();
    
    // Example for range based for loops:
    ret = ecall_range_based_for_loops_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for static_assert:
    ret = ecall_static_assert_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();
    
    // Example for virtual function controls : override, final, default, and delete
    ret = ecall_virtual_function_control_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for delegating_constructors:
    ret = ecall_delegating_constructors_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for std::function:
    ret = ecall_std_function_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();
    
    // Example for algorithms (std::all_of, std::any_of, std::none_of):
    ret = ecall_cxx11_algorithms_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for variadic_templates feature:
    ret = ecall_variadic_templates_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for SFINAE:
    ret = ecall_SFINAE_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for initializer_list:
    ret = ecall_initializer_list_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for rvalue:
    ret = ecall_rvalue_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for nullptr:
    ret = ecall_nullptr_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for new container classes (unordered_set, unordered_map, unordered_multiset, and unordered_multimap):
    ret = ecall_new_container_classes_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for tuple:
    ret = ecall_tuple_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

    // Example for shared_ptr:
    ret = ecall_shared_ptr_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();
    // Example for atomic:
    ret = ecall_atomic_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

//The following threads are part of mutex demo
	std::thread t1(demo_counter_without_mutex);
	std::thread t2(demo_counter_without_mutex);
	std::thread t3(demo_counter_without_mutex);
	t1.join();
	t2.join();
	t3.join();
	ret = ecall_print_final_value_no_protection(global_eid);
	if (ret != SGX_SUCCESS)
		abort();
	
	
	
//The following threads are part of mutex demo
    std::thread tm1(demo_counter_mutex);
    std::thread tm2(demo_counter_mutex);
    std::thread tm3(demo_counter_mutex);
    tm1.join();
    tm2.join();
    tm3.join();
    ret = ecall_print_final_value_mutex_demo(global_eid);
    if (ret != SGX_SUCCESS)
        abort();

//The following threads are part of condition variable demo
    std::thread th1(demo_cond_var_run);
    std::thread th2(demo_cond_var_load);
    th2.join();
    th1.join();

}

