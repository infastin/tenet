# frozen_string_literal: true
# typed: strict

require "sorbet-runtime"

class Task2::DisjointSet
	extend T::Sig

	sig { void }
	def initialize 
		@roots = T.let(Array[], T::Array[Integer])
		@ranks = T.let(Array[], T::Array[Integer])
	end

	sig { params(x: Integer).void }
	def make_set(x)
		@roots[x] = x
		@ranks[x] = 0
	end

	sig { params(x: Integer).returns(Integer) }
	def find(x)
		change_root = []

		while @roots[x] != x
			change_root << x
			x = T.must(@roots[x])
		end

		for i in change_root
			@roots[i] = x
		end

		return x
	end

	sig { params(x: Integer, y: Integer).void }
	def unite(x, y)
		x_root = find(x)
		y_root = find(y)

		if T.must(@ranks[x_root]) > T.must(@ranks[y_root])
			@roots[y_root] = x_root
		else
			@roots[x_root] = y_root

			if T.must(@ranks[x_root]) == T.must(@ranks[y_root])
				@ranks[y_root] = T.must(@ranks[y_root]) + 1
			end
		end
	end
end
