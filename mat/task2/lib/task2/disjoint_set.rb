# frozen_string_literal: true
# typed: strict

require "sorbet-runtime"

module Task2
	class DisjointSet
		extend T::Sig

		sig { params(size: T.nilable(Integer)).void }
		def initialize(size: nil)
			if size.nil?
				@roots = Array[]
				@ranks = Array[]
			else
				@roots = Array.new(size) { |i| i }
				@ranks = Array.new(size, 0)
			end

			@roots = T.let(@roots, T::Array[Integer])
			@ranks = T.let(@ranks, T::Array[Integer])
		end

		sig { params(v: Integer).void }
		def make_set(v)
			@roots[v] = v
			@ranks[v] = 0
		end

		sig { params(v: Integer).returns(Integer) }
		def find(v)
			raise "No such element: #{v}" if @roots[v].nil?

			change_root = []

			while @roots[v] != v
				change_root << v
				v = T.must(@roots[v])
			end

			for i in change_root
				@roots[i] = v
			end

			v
		end

		sig { returns(T::Boolean) }
		def coherent?
			coherent = T.let(true, T::Boolean)
			root = find(T.must(@roots[0]))

			1.upto(@roots.length - 1) do |i|
				if find(i) != root
					coherent = false
				end
			end

			coherent
		end

		sig { params(v: Integer).returns(Integer) }
		def [](v)
			return find(v)
		end

		sig { params(u: Integer, v: Integer).void }
		def unite(u, v)
			raise "No such element: #{u}" if @roots[u].nil?
			raise "No such element: #{v}" if @roots[v].nil?

			u_root = find(u)
			v_root = find(v)

			if T.must(@ranks[u_root]) > T.must(@ranks[v_root])
				@roots[v_root] = u_root
			else
				@roots[u_root] = v_root

				if T.must(@ranks[u_root]) == T.must(@ranks[v_root])
					@ranks[v_root] = T.must(@ranks[v_root]) + 1
				end
			end
		end
	end
end
