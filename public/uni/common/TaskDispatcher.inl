#pragma once

namespace uni
{
namespace common
{
template < typename F >
void
TaskDispatcher::cancel( const std::string& thread_pool_name, RequestId request_id, F&& f )
{
    m_impl.cancel_impl( thread_pool_name, request_id, std::forward< F >( f ) );
}

template < typename F, typename... Args >
void
TaskDispatcher::dispatch( const std::string& thread_pool_name, F&& f, Args&&... args )
{
    m_impl.dispatch_impl(
        thread_pool_name, std::forward< F >( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
void
TaskDispatcher::dispatch( const std::string& thread_pool_name,
                          Object* object,
                          MemberFnPtr< Object, R, Params... > f,
                          Args&&... args )
{
    m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
void
TaskDispatcher::dispatch( const std::string& thread_pool_name,
                          Object* object,
                          ConstMemberFnPtr< Object, R, Params... > f,
                          Args&&... args )
{
    m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
void
TaskDispatcher::dispatch( const std::string& thread_pool_name,
                          Object* object,
                          MemberFnPtrWrapper< R ( Object::* )( Params... ) > f,
                          Args&&... args )
{
    m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
void
TaskDispatcher::dispatch( const std::string& thread_pool_name,
                          Object* object,
                          MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                          Args&&... args )
{
    m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
void
TaskDispatcher::dispatch( const std::string& thread_pool_name,
                          const std::shared_ptr< Object >& object,
                          MemberFnPtrWrapper< R ( Object::* )( Params... ) > f,
                          Args&&... args )
{
    m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
void
TaskDispatcher::dispatch( const std::string& thread_pool_name,
                          const std::shared_ptr< Object >& object,
                          MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                          Args&&... args )
{
    m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename F, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name, F&& f, Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, std::forward< F >( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     Object* object,
                                     MemberFnPtrWrapper< R ( Object::* )( Params... ) > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     Object* object,
                                     MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     Object* object,
                                     MemberFnPtr< Object, R, Params... > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     Object* object,
                                     ConstMemberFnPtr< Object, R, Params... > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     const std::shared_ptr< Object >& object,
                                     MemberFnPtrWrapper< R ( Object::* )( Params... ) > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     const std::shared_ptr< Object >& object,
                                     MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     const std::shared_ptr< Object >& object,
                                     MemberFnPtr< Object, R, Params... > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
bool
TaskDispatcher::dispatch_or_execute( const std::string& thread_pool_name,
                                     const std::shared_ptr< Object >& object,
                                     ConstMemberFnPtr< Object, R, Params... > f,
                                     Args&&... args )
{
    return m_impl.dispatch_or_execute_impl(
        thread_pool_name, object, f, std::forward< Args >( args )... );
}

}  // namespace common
}  // namespace uni
