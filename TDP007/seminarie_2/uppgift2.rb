require 'rexml/streamlistener'
require 'rexml/document'

class Event
  def initialize()
    @summary, @location, @posted_by, @date, @description = ''
    @region, @street_address, @org_fn = ''
  end

  def to_string
    puts "-" * 75
    puts "Event name: #{summary}"
    puts "Organizer: #{org_fn}"
    puts "Location: #{location}, #{region}"
    puts "Street address: #{street_address}"
    puts "Posted by: #{posted_by}"
    puts "Event date: #{date}"
    puts "Event description:\n#{description}"
    puts "-" * 75
  end

  attr_accessor :summary
  attr_accessor :location
  attr_accessor :posted_by
  attr_accessor :date
  attr_accessor :description
  attr_accessor :region
  attr_accessor :street_address
  attr_accessor :date
  attr_accessor :org_fn
end

class MyListener
  include REXML::StreamListener

  def initialize()
    @classlist = []
    @current_assign
    @currentevent
    @listen_text = false
    @description = false

    @listensummary
  end

  def tag_start(name, attrs)
    if attrs.key('vevent')
      @listen = true
      @currentevent = Event.new
    end

    if @listen && attrs.key('summary')
      @listen_text = true
      @current_assign = "summary"
    end

    if @listen && attrs.key('dtstart')
      @listen_text = true
      @current_assign = "date"
    end

    if @listen && attrs.key('locality')
      @listen_text = true
      @current_assign = "locality"
    end

    if @listen && attrs.key('region')
      @listen_text = true
      @current_assign = "region"
    end

    if @listen && attrs.key('street-address')
      @listen_text = true
      @current_assign = "street address"
    end

    if @listen && attrs.key('org fn')
      @listen_text = true
      @current_assign = "org fn"
    end

    if @listen && attrs.key('userLink ')
      @listen_text = true
      @current_assign = "posted by"
    end

    if @listen && attrs.key('description')
      @description = true
    end

    if @listen && @description && name == "p"
      @listen_text = true
      @current_assign = "description"

      @listen = false
    end
  end

  def text(text)
      if @listen_text
        case @current_assign
        when 'summary'
          @currentevent.summary = text
        when 'date'
          @currentevent.date = text
        when 'locality'
          @currentevent.location = text
        when 'region'
          @currentevent.region = text
        when 'street address'
          @currentevent.street_address = text
        when 'org fn'
          @currentevent.org_fn = text
        when 'posted by'
          @currentevent.posted_by = text
        when 'description'
          @currentevent.description = text

          @classlist << @currentevent
        end

        @listen_text = false
      end
  end

  def to_string
    @classlist.each {|x| puts x.to_string}
  end
end

def listener_test
  lst = MyListener.new
  src = File.new './events.html'
  REXML::Document.parse_stream(src, lst)
  lst.to_string
end

#listener_test
