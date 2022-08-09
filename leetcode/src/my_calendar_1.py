class MyCalendar:
	def __init__(self):
		## Needs to be ordered for binary search
		self.bookings = []


	def book(self, start: int, end: int) -> bool:
		success, index = self._search(start, end)
		if success:
			self.bookings.insert(index, (start, end))
			return True
		return False
	
	def _search(self, start: int, end: int) -> Tuple[bool, Optional[int]]:
		if len(self.bookings) == 0:
			return True, 0
		elif len(self.bookings) == 1:
			bookedStart, bookedEnd = self.bookings[0]
			if (bookedStart <= start < bookedEnd) or (bookedStart < end <= bookedEnd):
				return False, None
			elif start < bookedStart:
				return True, 0
			else:
				return True, 1
				
		left = 0
		right = len(self.bookings) - 1
		
		while left < right:
			center = (left + right) // 2
			
			## check if center is a hit
			bookedStart, bookedEnd = self.bookings[center]
			if (bookedStart <= start < bookedEnd) or (bookedStart < end <= bookedEnd):
				return False, None
			elif (start <= bookedStart) and (bookedEnd <= end):
				return False, None
			elif start < bookedStart:
				right = center - 1
			elif start >= bookedEnd:
				left = center + 1
			else:
				raise Exception("This should have handled all the cases!!!")
					
					## If we've got to this point we're down to two
					## -- At this point, left == right
			if start >= self.bookings[left][1] and (left == len(self.bookings) - 1 or end <= self.bookings[left+1][0]):
				return True, left + 1
			elif end <= self.bookings[left][0] and (left == 0 or start >= self.bookings[left-1][0]):
				return True, left
			else:
				return False, None
							
							
							
							
# Your MyCalendar object will be instantiated and called as such:
# obj = MyCalendar()
# param_1 = obj.book(start,end)

