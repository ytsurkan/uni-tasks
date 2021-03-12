#pragma once

namespace uni
{
namespace common
{
template < typename Object, typename R, typename... Params, typename... Args >
void
TaskDispatcherBasic::dispatch( const std::string& thread_pool_name,
                               Object* object,
                               MemberFnPtr< Object, R, Params... > f,
                               Args&&... args )
{
    m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcherBasic::dispatch_or_execute( const std::string& thread_pool_name,
                                          Object* object,
                                          MemberFnPtr< Object, R, Params... > f,
                                          Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

}  // namespace common
}  // namespace uni
