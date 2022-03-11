# RDK

Ruud's Development Kit

## Examples

### TaskScheduler

    class MyExecutor
    {
    public:
        virtual void handleAsyncUpdate() = 0;
        void triggerAsyncUpdate()
        {
            // For the sake of demo purposes we're going to call back immediatly (which makes this class not really async).
            handleAsyncUpdate();
        }
    }

    rdk::TaskScheduler<MyExecutor> taskScheduler;
    taskScheduler.scheduleWork([]{ std::cout << "Hello world" << std::endl; });

