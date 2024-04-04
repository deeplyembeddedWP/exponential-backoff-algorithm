# Exponential backoff retries with jitter
A simple exponential backoff library written in C and easily portable to other languages. 

On operation failures, this module ensures to double the time interval between retries slowing down the rate of retries. It also adds jitter to the intervals between each retry to ensure the timing of operations doesn't collide when multiple clients are sending requests to the server, this ensures the server doesn't get overwhelmed with requests. Below are the variables and usage.

### exponential_retry_config_t
```
delay_base_s        \\ The base delay is used between retries.
```
```
delay_backoff_max_s  \\ The delay used between retries when all retries have exhausted.
```
```
jitter_percentage    \\ Percentage of jitter to the retry interval.
```
```
retry_count_max      \\ Maximum number of exponential retries before retrying every delay_backoff_max_s seconds
```
### exponential_retry_t
```
retry_counter          \\ Internally used to keep track of the retry counts
```
```
total_jitter_period_s  \\ Internally used to keep track of the cumulative jitter added between retries 
```
### Functions
```
exponential_retry_init(...)  \\ Initializes the context
```
```
exponential_retry_next(...)  \\ Calculates and returns the retry period to be used 
```
> [!WARNING]
>delay_backoff_max_s should always have a valid (>0) value, if invalid (<=0), subsequent retry intervals are based off delay_base_s.

Enjoy :slightly_smiling_face:

