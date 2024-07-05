# example.rb


# the following is invalid
# $- = "TESTING"
# puts $-

$-y = "TESTING1"
puts $-y

a = nil
puts a.class


$GLOBAL = [2, 4, 6]
_waka = 1
puts "#{_waka + 1}"

nc = Classical.new(a)

# classical get/set
class Classical
    #get
    def member1
        return @member1
        # or just @member1
    end

    # set
    def member1=(member1)
        @member1 = member1
    end

    def initialize(a)
        @member1 = a
    end
end

class WithAccessors 
    attr_accessor :member1, :member2

    def initialize(a, b)
        @member1 = a
        @member2 = b
    end
end

accessorExample = WithAccessors.new(2, "welp")
puts "#{accessorExample.member1}" + " " + accessorExample.member2

$GLOBAL.each { |i| puts i+1 }
it = $GLOBAL.each
puts it.next + it.next + it.next

class SelfCallCounter
    def initialize
        @@staticMember = 0
    end

    def countedCall
        @@staticMember += 1
        puts "staticMember called #{@@staticMember} time(s)."
    end
end

s = SelfCallCounter.new
s2 = SelfCallCounter.new

s.countedCall
s2.countedCall