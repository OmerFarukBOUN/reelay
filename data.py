
from time import sleep

# Used for updating the budgets when an interrupt occurs
def update_all_budgets(id):
	for int_id, interrupt in interrupt_events.items():
		if int_id != id:
			interrupt.current_budget -= interrupt.leak_func()
		else:
			interrupt.current_budget -= interrupt.cup_amount
		if interrupt.current_budget < interrupt.wcet:
			mask(int_id)

# Used for making the interrupt unuable int the current frame
def mask(id):
	interrupt_events[id].can_be_unmasked = False
	# Additional logic to mask the interrupt in the system

# Used for unmasking and resetting the interrupts
def reset_interrupts():
	for interrupt in interrupt_events.values():
		interrupt.current_budget = interrupt.budget
		interrupt.can_be_unmasked = True
		# Additional logic to reset the interrupt in the system

# Used for masking all interrupts in the system
def mask_all_interrupts():
	for interrupt in interrupt_events.values():
		# Additional logic to mask all interrupts in the system
		pass

# Used for unmasking all unmaskable interrupts in the system
def unmask_all_interrupts():
	for interrupt in interrupt_events.values():
		if interrupt.can_be_unmasked:
			# Additional logic to unmask the interrupt in the system
			pass
	

class Partition:
    def __init__(self, type, duration, task):
        self.type = type # "partition" or "slack"
        self.duration = duration
        self.task = task # Function pointer for task

class Interrupt_Event:
    def __init__(self, id, ISR, budget, leak_func, cup_amount, wcet):
        self.id = id
        self.ISR = lambda: (update_all_budgets(self.id), ISR()) if self.current_budget > wcet else mask(self.id) # Function pointer for ISR
        self.budget = budget
        self.leak_func = leak_func # In the article it was stated as a monotolically decreasing function
        self.cup_amount = cup_amount # Amount of used budget when calling the ISR
        self.current_budget = budget # Current budget of the ISR
        self.wcet = wcet # Worst case execution time of the ISR
        self.can_be_unmasked = True # Flag to check if the ISR can be unmasked


# Example
fixed_schedule = [
    Partition("partition", 20, lambda: print("Executing partition task")),
    Partition("slack", 15, sleep(15)),
    Partition("partition", 25, lambda: print("Executing another partition task")),
	Partition("slack", 10, sleep(10))
]

interrupt_events = {
    0: Interrupt_Event(0, lambda: print("Handling interrupt 1"), 10, 0.1, 4, 1),
    1: Interrupt_Event(1, lambda: print("Handling interrupt 2"), 5, 0.2, 2, 0.5),
}

major_frame_duration = sum(entry.duration for entry in fixed_schedule)
# Main scheduling loop

init()
while True:
	reset_interrupts()
	for partition in fixed_schedule:
		start_timer(partition.duration) # The partition changes with the timer, we are simplifying it
		if partition.type == "partition":
			mask_all_interrupts()
			partition.task()
		elif partition.type == "slack":
			unmask_all_interrupts()