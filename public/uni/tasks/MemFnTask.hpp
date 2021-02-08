#pragma once

#include "uni/tasks/TaskBase.hpp"
#include "uni/utils/Utils.hpp"

namespace uni
{
template < typename Object, typename F, typename... Args >
class MemFnTask : public TaskBase
{
public:
    using PromiseType = std::promise< void >;

public:
    MemFnTask( Object* object, F f, Args&&... args )
        : m_object{object}
        , m_function{f}
        , m_args{std::forward< Args >( args )...}
    {}

    ~MemFnTask() override = default;
    MemFnTask( const MemFnTask& ) = delete;
    MemFnTask& operator=( const MemFnTask& ) = delete;
    MemFnTask(MemFnTask &&) noexcept = default;
    MemFnTask &operator=(MemFnTask &&) noexcept = default;

private:
    void
    call_impl( ) override
    {
        call_impl( m_args, std::index_sequence_for< Args... >( ) );
    }

private:
    template < typename Tuple, size_t... Indices >
    void
    call_impl( Tuple&& tuple, std::index_sequence< Indices... > )
    {
        utils::invoke( std::forward< F >( m_function ),
                       *m_object,
                       std::forward< Args >( std::get< Indices >( tuple ) )... );
    }

private:
    Object *m_object{nullptr};
    F m_function;
    std::tuple< std::decay_t< Args >... > m_args;
};

template < typename Object, typename F, typename... Args >
class MemFnTask< std::shared_ptr< Object >, F, Args... > : public TaskBase
{
public:
    MemFnTask( std::shared_ptr< Object >&& object, F f, Args&&... args )
        : m_object{std::move( object )}
        , m_function{f}
        , m_args{std::forward< Args >( args )...}
    {}

    MemFnTask( const std::shared_ptr< Object >& object, F f, Args&&... args )
        : m_object{object}
        , m_function{f}
        , m_args{std::forward< Args >( args )...}
    {}

    ~MemFnTask() override = default;
    MemFnTask( const MemFnTask& ) = delete;
    MemFnTask& operator=( const MemFnTask& ) = delete;
    MemFnTask(MemFnTask &&) noexcept = default;
    MemFnTask &operator=(MemFnTask &&) noexcept = default;

private:
    void
    call_impl( ) override
    {
        call_impl( m_args, std::index_sequence_for< Args... >( ) );
    }

private:
    template < typename Tuple, size_t... Indices >
    void
    call_impl( Tuple&& tuple, std::index_sequence< Indices... > )
    {
        utils::invoke( std::forward< F >( m_function ),
                       *m_object,
                       std::forward< Args >( std::get< Indices >( tuple ) )... );
    }

private:
    std::shared_ptr<Object> m_object;
    F m_function;
    std::tuple<std::decay_t<Args>...> m_args;
};

}  // namespace uni
